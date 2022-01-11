#include "segmentation.h"
#include "utility.h"
#include "contour_detection.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include <set>

using std::cout;
using std::endl;
using cv::Mat;

namespace segmentation{

const utility::DEBUG_LEVEL DEBUG = utility::DEBUG_ZERO;

void Segmenter::segmentVideo() {
  cv::VideoCapture cap(0);
  Mat img;
  if (cap.isOpened() == false) {
    cout << "cannot open webcam\n";
    exit(1);
  }

  Mat markers;
  while (true) {
    cap.read(img);
    Mat segmentedImg = segmentImg(img);
    cv::flip(segmentedImg, segmentedImg, 1);
    utility::showImg(segmentedImg);
    cv::waitKey(1);
  }
}

Mat Segmenter::preprocessImg(const Mat &img) {
  Mat imgGray, thresh;
  Mat kernel = (cv::Mat_<float>(3, 3) << 
                1, 1, 1, 
                1, -8, 1, 
                1, 1, 1);
  Mat imgLaplacian;
  cv::filter2D(img, imgLaplacian, CV_32F, kernel);
  Mat sharp;
  img.convertTo(sharp, CV_32F);
  Mat imgResult = sharp - imgLaplacian;
  imgResult.convertTo(imgResult, CV_8UC3);
  imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
  utility::showImg(imgLaplacian, "laplacian", DEBUG);
  utility::showImg(imgResult, "sharp result", DEBUG);

  cv::cvtColor(imgResult, imgGray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(imgGray, imgGray, cv::Size(3, 3), 3, 0);
  Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  cv::dilate(imgGray, imgGray, kernel2);
  cv::threshold(imgGray, thresh, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

  utility::showImg(imgGray, "gray image", DEBUG);
  utility::showImg(thresh, "thresh", DEBUG);

  Mat dist_transform;
  cv::distanceTransform(thresh, dist_transform, cv::DIST_L2, 3);

  cv::normalize(dist_transform, dist_transform, 0, 1.0, cv::NORM_MINMAX);
  utility::showImg(dist_transform, "dist transform", DEBUG);

  cv::threshold(dist_transform, dist_transform, 0.4, 1.0, cv::THRESH_BINARY);

  Mat kernel1 = Mat::ones(3, 3, CV_8U);
  cv::dilate(dist_transform, dist_transform, kernel1);
  utility::showImg(dist_transform, "dilated dist transform", DEBUG);

  Mat dist_8u;
  dist_transform.convertTo(dist_8u, CV_8U);

  cv::findContours(dist_8u, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  markers = Mat::zeros(dist_8u.size(), CV_32S);
  for(size_t i = 0; i<contours.size(); ++i) {
    cv::drawContours(markers, contours, static_cast<int>(i), cv::Scalar(static_cast<int>(i)+1), -1);
  }

  cv::circle(markers, cv::Point(5, 5), 3, cv::Scalar(255), -1);
  Mat markers8u;
  markers.convertTo(markers8u, CV_8U, 10);
  utility::showImg(markers8u, "markers before watershed", DEBUG);

  cv::watershed(img, markers);
  markers.convertTo(markers8u, CV_8U, 10);
  utility::showImg(markers8u, "markers after watershed", DEBUG);
  return markers;
}

void Segmenter::removeBadContours(const vector<vector<cv::Point>> &contours, vector<int> &indices, int rows, int cols) {
  for(int i = contours.size() - 1; i >= 0; --i) {
    if(cv::contourArea(contours[i]) > 0.6*rows*cols) {
      indices.erase(indices.begin() + i);
    }
  }
}

const Mat &Segmenter::getMarkers() {
  return markers;
}

const vector<vector<Point>> &Segmenter::getContours() {
  return contours;
}

Mat Segmenter::segmentImg(const Mat &img) {
  utility::showImg(img, "original image", DEBUG);
  Mat markers = preprocessImg(img);

  Mat dst = img.clone();

  vector<int> indices(contours.size(), 0);
  for(int i=1; i<=contours.size(); ++i) indices[i-1] = i;
  vector<vector<cv::Point>> contours2;
  for(auto index : indices) {
    Mat temp = (markers==index);
    contours2.push_back(contour_detection::getContours(temp)[0]);
  }
  removeBadContours(contours2, indices, img.rows, img.cols);

  for(auto it : indices) {
    Mat temp = (markers == it);
    contour_detection::outlineContours(dst, temp);
  }

  if(DEBUG == utility::DEBUG_THREE) {
    utility::showImg(markers == -1, "-1");
    for(int i=1; i<=contours.size(); ++i) {
      utility::showImg(markers == i, std::to_string(i));
    }
    utility::showImg(markers == 255, "255");
  }

  utility::showImg(dst, "final", DEBUG);

  return dst;
}

}