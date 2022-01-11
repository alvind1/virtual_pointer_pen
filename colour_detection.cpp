#include "colour_detection.h"
#include "utility.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using cv::imread;
using cv::Mat;
using cv::Scalar;
using cv::createTrackbar;
using std::cout;
using std::endl;
using std::string;
using utility::hsvVals;
using utility::MAX_HSV;
using utility::MIN_HSV;

namespace colour_detection {

Mat ColourDetector::convertImageHSV(const Mat &img) {
  cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
  return imgHSV;
}

const Mat &ColourDetector::getImgHSV() {
  return imgHSV;
}

Mat ColourDetector::findColour(const Mat &img, const hsvVals min, const hsvVals max) {
  Scalar lower(min.hue, min.sat, min.val);
  Scalar upper(max.hue, max.sat, max.val);
  inRange(img, lower, upper, mask);
  return mask;
}

void ColourDetector::imageColourPicker(const Mat &img) {
  Mat imgHSV = convertImageHSV(img), mask;
  imshow("Image", img);
  imshow("Image HSV", imgHSV);

  hsvVals hsv_min = utility::MIN_HSV;
  hsvVals hsv_max = utility::MAX_HSV;
  utility::createHSVTrackbars(hsv_min, hsv_max);

  while (1) {
    mask = findColour(imgHSV, hsv_min, hsv_max);
    cout << hsv_min.hue << ' ' << hsv_min.sat << ' ' << hsv_min.val << endl;
    cout << hsv_max.hue << ' ' << hsv_max.sat << ' ' << hsv_max.val << endl;
    cout << endl;
    imshow("Image Colours", mask);
    cv::waitKey(1);
  }
}

void ColourDetector::videoColourPicker() {
  cv::VideoCapture cap(0);
  Mat img, imgHSV, mask;
  if (cap.isOpened() == false) {
    std::cout << "cannot open webcam\n";
    exit(1);
  }

  hsvVals hsv_min = MIN_HSV;
  hsvVals hsv_max = MAX_HSV;
  utility::createHSVTrackbars(hsv_min, hsv_max);

  while (true) {
    cap.read(img);
    cout << hsv_min.hue << ' ' << hsv_min.sat << ' ' << hsv_min.val << endl;
    cout << hsv_max.hue << ' ' << hsv_max.sat << ' ' << hsv_max.val << endl;
    cout << endl;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    mask = findColour(imgHSV, hsv_min, hsv_max);
    cv::imshow("Image", img);
    cv::imshow("Image Colours", mask);
    cv::waitKey(1);
  }
}

void ColourDetector::imageColourDetection(string path) {
  Mat img = cv::imread(path);
  Mat imgHSV = convertImageHSV(img);
  Mat mask = findColour(imgHSV, {129, 54, 0}, {179, 255, 255});  // Tesla colours
  cv::imshow("Image", img);
  cv::imshow("Image HSV", imgHSV);
  cv::imshow("Image Colours", mask);
  cv::waitKey(0);
}

}  // namespace colour_detection