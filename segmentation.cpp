#include "segmentation.h"
#include "utility.h"
#include "contour_detection.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include <set>

using cv::Mat;
using std::cout;
using std::endl;

namespace segmentation{
  void segmentVideo() {
    cv::VideoCapture cap(0);
    Mat img;
    if (cap.isOpened() == false) {
      cout << "cannot open webcam\n";
      exit(1);
    }

    vector<vector<cv::Point>> contours;
    Mat markers;
    while (true) {
      cap.read(img);
      Mat segmentedImg  = segmentImg(img, contours, markers);
      cv::flip(segmentedImg, segmentedImg, 1);
      utility::show_img(segmentedImg);
      cv::waitKey(1);
    }
  }

  void showSegmentedImg(const string &path) {
    Mat img = cv::imread(utility::IMAGE_PATH+"mydesk.jpg");
    vector<vector<cv::Point>> contours;
    Mat markers;
    Mat segmentedImg = segmentImg(img, contours, markers);
    utility::showImgWait(segmentedImg);
  }

  Mat preprocessImg(Mat &img) {
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

    cv::cvtColor(imgResult, imgGray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(imgGray, imgGray, cv::Size(3, 3), 3, 0);
    Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(imgGray, imgGray, kernel2);
    cv::threshold(imgGray, thresh, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    Mat dist_transform;
    cv::distanceTransform(thresh, dist_transform, cv::DIST_L2, 3);

    cv::normalize(dist_transform, dist_transform, 0, 1.0, cv::NORM_MINMAX);

    cv::threshold(dist_transform, dist_transform, 0.4, 1.0, cv::THRESH_BINARY);

    Mat kernel1 = Mat::ones(3, 3, CV_8U);
    cv::dilate(dist_transform, dist_transform, kernel1);

    Mat dist_8u;
    dist_transform.convertTo(dist_8u, CV_8U);
    return dist_8u;
  }

  void removeBadContours(const vector<vector<cv::Point>> &contours, vector<int> &indices, int rows, int cols, const Mat &img) {
    for(int i = contours.size() - 1; i >= 0; --i) {
      if(cv::contourArea(contours[i]) > 0.6*rows*cols) {
        indices.erase(indices.begin() + i);
      }
    }
  }

  Mat segmentImg(Mat &img, vector<vector<cv::Point>> &contours, Mat &markers) {
    utility::show_img(img, "original image");
    Mat dist_8u = preprocessImg(img);

    cv::findContours(dist_8u, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    markers = Mat::zeros(dist_8u.size(), CV_32S);
    for(size_t i = 0; i<contours.size(); ++i) {
      cv::drawContours(markers, contours, static_cast<int>(i), cv::Scalar(static_cast<int>(i)+1), -1);
    }

    cv::circle(markers, cv::Point(5, 5), 3, cv::Scalar(255), -1);
    Mat markers8u;
    markers.convertTo(markers8u, CV_8U, 10);

    cv::watershed(img, markers);
    markers.convertTo(markers8u, CV_8U, 10);

    Mat dst = img.clone();

    vector<int> indices(contours.size(), 0);
    for(int i=1; i<=contours.size(); ++i) indices[i-1] = i;
    vector<vector<cv::Point>> contours2;
    for(auto index : indices) {
      Mat temp = (markers==index);
      contours2.push_back(contour_detection::getContours(temp)[0]);
    }
    segmentation::removeBadContours(contours2, indices, img.rows, img.cols, img);

    for(auto it : indices) {
      Mat temp = (markers == it);
      contour_detection::outlineContours(dst, temp);
    }

    return dst;
  }

  void testSegmentImg() {
    Mat img, imgGray, thresh;
    img = cv::imread(utility::IMAGE_PATH + "mydesk.jpg");
    utility::show_img(img, "original image");

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

    utility::show_img(imgLaplacian, "laplacian");
    utility::show_img(imgResult, "sharp result");

    cv::cvtColor(imgResult, imgGray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(imgGray, imgGray, cv::Size(3, 3), 3, 0);
    Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(imgGray, imgGray, kernel2);
    cv::threshold(imgGray, thresh, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    utility::show_img(imgGray, "gray image");
    utility::show_img(thresh, "thresh");

    Mat dist_transform;
    cv::distanceTransform(thresh, dist_transform, cv::DIST_L2, 3);

    cv::normalize(dist_transform, dist_transform, 0, 1.0, cv::NORM_MINMAX);
    utility::show_img(dist_transform, "dist transform");

    cv::threshold(dist_transform, dist_transform, 0.4, 1.0, cv::THRESH_BINARY);

    Mat kernel1 = Mat::ones(3, 3, CV_8U);
    cv::dilate(dist_transform, dist_transform, kernel1);
    utility::show_img(dist_transform, "dilated dist transform");

    Mat dist_8u;
    dist_transform.convertTo(dist_8u, CV_8U);

    vector<vector<cv::Point>> contours;
    cv::findContours(dist_8u, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    Mat markers = Mat::zeros(dist_transform.size(), CV_32S);
    for(size_t i = 0; i<contours.size(); ++i) {
      cv::drawContours(markers, contours, static_cast<int>(i), cv::Scalar(static_cast<int>(i)+1), -1);
    }

    cv::circle(markers, cv::Point(5, 5), 3, cv::Scalar(255), -1);
    Mat markers8u;
    markers.convertTo(markers8u, CV_8U, 100);
    utility::show_img(markers8u, "markers before watershed");

    cv::watershed(img, markers);
    markers.convertTo(markers8u, CV_8U, 100);
    utility::show_img(markers8u, "markers after watershed");

    Mat dst = img;
    for(int i=1; i<=contours.size(); ++i) {
      Mat temp = (markers == i);
      contour_detection::outlineContours(dst, temp);
    }
    utility::show_img(dst, "final");

    //cout << contours.size() << endl;
    //utility::show_img(markers == -1, "-1");
    //for(int i=1; i<=contours.size(); ++i) {
    //  utility::show_img(markers == i, std::to_string(i));
    //}
    //utility::show_img(markers == 255, "255");

    cv::waitKey(0);
  }

  void segment2() {
    Mat img, imgGray, thresh;
    img = cv::imread(utility::IMAGE_PATH + "mydesk.jpg");
    utility::show_img(img, "original image");
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
    cv::threshold(imgGray, thresh, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    utility::show_img(imgGray, "gray image");
    utility::show_img(thresh, "thresh");

    Mat dist_transform;
    cv::distanceTransform(thresh, dist_transform, cv::DIST_L2, 3);

    cv::normalize(dist_transform, dist_transform, 0, 1.0, cv::NORM_MINMAX);
    utility::show_img(dist_transform, "dist transform");

    cv::threshold(dist_transform, dist_transform, 0.4, 1.0, cv::THRESH_BINARY);

    Mat kernel1 = Mat::ones(3, 3, CV_8U);
    cv::dilate(dist_transform, dist_transform, kernel1);
    utility::show_img(dist_transform, "dilated dist transform");

    Mat dist_8u;
    dist_transform.convertTo(dist_8u, CV_8U);

    vector<vector<cv::Point>> contours;
    cv::findContours(dist_8u, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    Mat markers = Mat::zeros(dist_transform.size(), CV_32S);
    for(size_t i = 0; i<contours.size(); ++i) {
      cv::drawContours(markers, contours, static_cast<int>(i), cv::Scalar(static_cast<int>(i)+1), -1);
    }

    cv::circle(markers, cv::Point(5, 5), 3, cv::Scalar(255), -1);
    Mat markers8u;
    markers.convertTo(markers8u, CV_8U, 10);
    utility::show_img(markers8u, "markers");

    cv::watershed(img, markers);
    markers.convertTo(markers8u, CV_8U, 10);
    utility::show_img(markers8u, "after watershed");

    Mat mark;
    markers.convertTo(mark, CV_8U);
    cv::bitwise_not(mark, mark);

    vector<cv::Vec3b> colors;
    for(size_t i=0; i<contours.size(); ++i) {
      int b = cv::theRNG().uniform(0, 256);
      int g = cv::theRNG().uniform(0, 256);
      int r = cv::theRNG().uniform(0, 256);
      colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    for(int i=0; i<markers.rows; ++i) {
      for(int j=0; j<markers.cols; ++j) {
        int index = markers.at<int>(i, j);
        if(index > 0 && index <= static_cast<int>(contours.size())) {
          dst.at<cv::Vec3b>(i, j) = colors[index-1];
        }
      }
    }
    utility::show_img(dst, "final");

    cv::waitKey(0);
  }

  void segment3() {
    Mat img, imgGray, ret, opening;
    img = cv::imread(utility::IMAGE_PATH + "coins.jpg");
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
    cv::threshold(imgGray, ret, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

    Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(ret, opening, cv::MORPH_OPEN, kernel);

    Mat sure_bg;
    cv::dilate(opening, sure_bg, kernel);

    Mat dist_transform;
    cv::distanceTransform(opening, dist_transform, cv::DIST_L2, 5);
    Mat sure_fg;
    double minVal, maxVal;
    cv::minMaxLoc(dist_transform, &minVal, &maxVal);
    cv::threshold(dist_transform, sure_fg, 0.7*maxVal, 255, 0);

    Mat unknown;
    sure_fg.convertTo(sure_fg, CV_8UC1);
    cv::subtract(sure_bg, sure_fg, unknown);

    utility::show_img(sure_bg, "sure_bg");
    utility::show_img(sure_fg, "sure_fg");
    utility::show_img(unknown, "unknown");

    Mat markers;
    cv::connectedComponents(sure_fg, markers);
    markers.convertTo(markers, CV_32S);

    markers += 1;
    
    vector<pair<int, int>> temp;
    for(int i=0; i<unknown.rows; ++i) {
      for(int j=0; j<unknown.cols; ++j) {
        if(img.at<uchar>(i, j) == 255) {
          markers.at<uchar>(i, j) = 0;
        }
      }
    }

    cv::watershed(img, markers);
    for(int i=0; i<markers.rows; ++i) {
      for(int j=0; j<markers.cols; ++j) {
        if(markers.at<int>(i, j) == -1) {
          img.at<cv::Vec3b>(i, j) = {255, 0, 0};
        }
      }
    }
    utility::showImgWait(img);
  }
}