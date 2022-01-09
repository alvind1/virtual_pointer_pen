#include "contour_detection.h"
#include "utility.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using cv::imshow;
using cv::Mat;
using cv::Point;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using utility::IMAGE_PATH;

namespace contour_detection {
void getContouredImg(const Mat &img, Mat &imgDil, bool showImage = false) {
  Mat imgGray, imgBlur, imgCanny;

  cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(imgGray, imgBlur, cv::Size(3, 3), 3, 0);
  cv::Canny(imgBlur, imgCanny, 25, 75);
  Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  cv::dilate(imgCanny, imgDil, kernel);

  if (showImage) {
    imshow("Image", img);
    imshow("Gray Image", imgGray);
    imshow("Blurred Image", imgBlur);
    imshow("Canny Image", imgCanny);
    imshow("Kernel", kernel);
    imshow("Dilate", imgDil);
  }
}

vector<vector<Point>> getContours(const Mat &img) {
  vector<vector<Point>> contours;
  vector<cv::Vec4i> hiearchy;
  cv::findContours(img, contours, hiearchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  return contours;
}

cv::RotatedRect drawRotatedRectangle(Mat &img, const cv::RotatedRect &rotatedRect) {
  cv::Point2f rotatedRectPoints[4];
  rotatedRect.points(rotatedRectPoints);
  vector<vector<Point>> tempV;
  tempV.push_back({});
  for(int i=0; i<4; ++i) tempV[0].push_back(rotatedRectPoints[i]);
  cv::drawContours(img, tempV, 0, cv::Scalar(23, 72, 237), 10);

  return rotatedRect;
}

cv::RotatedRect drawRotatedRectangle(Mat &img, const vector<Point> &contour) {
  cv::RotatedRect rotatedRect = cv::minAreaRect(contour);
  return drawRotatedRectangle(img, rotatedRect);
}

string getShapeType(const vector<cv::Point> &conPoly, const cv::Rect &boundedRect) {
  string objectType;
  int numObjSides = (int) conPoly.size();
  if (numObjSides == 3) {
    objectType = "triangle";
  } else if (numObjSides == 4) {
    float ratio = (float) boundedRect.width / (float)boundedRect.height;
    if (0.9 <= ratio && ratio <= 1.1) {
      objectType = "square";
    }
    else {
      objectType = "rect";
    }
  } else if (numObjSides > 4) {
    objectType = "circle";
  }
  return objectType;
}

void outlineContours(Mat &img, const Mat &imgDil, int areaThreshold) {
  vector<vector<Point>> contours = getContours(imgDil);
  vector<vector<Point>> conPoly(contours.size());

  for (int i = 0; i < contours.size(); ++i) {
    int area = cv::contourArea(contours[i]);
    if (area > areaThreshold) {
      float peri = cv::arcLength(contours[i], true);
      cv::approxPolyDP(contours[i], conPoly[i], 0.01 * peri, true);
      cv::drawContours(img, conPoly, i, cv::Scalar(255, 0, 255), 2);
    }
  }
}

void outlineShapes(Mat img, const Mat &imgDil, int areaThreshold) {
  vector<vector<Point>> contours = getContours(imgDil);
  vector<vector<Point>> conPoly(contours.size());
  vector<cv::Rect> boundedRect(contours.size());

  for (int i = 0; i < contours.size(); ++i) {
    int area = cv::contourArea(contours[i]);
    if (area > areaThreshold) {
      float peri = cv::arcLength(contours[i], true);
      cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
      cv::drawContours(img, conPoly, i, cv::Scalar(255, 0, 255), 10);

      boundedRect[i] = cv::boundingRect(conPoly[i]);
      cv::rectangle(img, boundedRect[i].tl(), boundedRect[i].br(),
                    cv::Scalar(0, 255, 0), 5);

      drawRotatedRectangle(img, contours[i]);

      string objectType = getShapeType(conPoly[i], boundedRect[i]);

      cv::putText(img, objectType, {boundedRect[i].x, boundedRect[i].y - 5}, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 69, 255), 1);
    }
  }
}

vector<Point> findLargestRectangle(const Mat &img) {
  vector<vector<Point>> contours = getContours(img);
  string objectType;

  int max_area = 100, biggest_contour_index = -1;
  for (int i = 0; i < contours.size(); ++i) {
    int area = cv::contourArea(contours[i]);
    if (area > max_area) {
      biggest_contour_index = i;
      max_area = area;
    }
  }
  
  if(biggest_contour_index < 0) return vector<Point>{};
  else return contours[biggest_contour_index];
}

cv::Point drawLargestRectangle(Mat img, const Mat &imgDil) {
  vector<Point> contour = findLargestRectangle(imgDil);
  if (contour.size() == 0) return Point{0, 0};

  vector<Point> conPoly;
  float peri = cv::arcLength(contour, true);
  cv::approxPolyDP(contour, conPoly, 0.02 * peri, true);
  cv::drawContours(img, vector<vector<Point>>{conPoly}, 0, cv::Scalar(255, 0, 255), 10);

  Point mypoint(0, 0);
  cv::Rect boundedRect;
  boundedRect = cv::boundingRect(conPoly);
  mypoint.x = boundedRect.x + boundedRect.width / 2;
  mypoint.y = boundedRect.y;
  cv::rectangle(img, boundedRect.tl(), boundedRect.br(), cv::Scalar(0, 255, 0), 5);

  return mypoint;
}

cv::RotatedRect drawLargestRotatedRectangle(Mat img, const Mat &imgDil) {
  vector<Point> contour = findLargestRectangle(imgDil);
  if(contour.size() == 0) return cv::RotatedRect();
  return drawRotatedRectangle(img, contour);
}

void detectContoursFromImg(Mat &img, int contouredAreaThreshold) {
  Mat imgDil;
  getContouredImg(img, imgDil, false);
  outlineContours(img, imgDil, contouredAreaThreshold);
}

void detectShapesFromImg(Mat &img) {
  Mat imgDil;
  getContouredImg(img, imgDil, false);
  outlineShapes(img, imgDil, 2000);
}

void detectContoursFromVideo() {
  cv::VideoCapture cap(0);
  Mat img;
  if (cap.isOpened() == false) {
    cout << "cannot open webcam\n";
    exit(1);
  }

  while (true) {
    cap.read(img);
    contour_detection::detectContoursFromImg(img, 500);
    cv::flip(img, img, 1);
    utility::show_img(img);
    cv::waitKey(1);
  }
}

void shapeDetection() {
  Mat img = cv::imread(IMAGE_PATH + "Shapes.jpg");
  contour_detection::detectShapesFromImg(img);
  utility::showImgWait(img);
}
}  // namespace contour_detection