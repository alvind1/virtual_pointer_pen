#include "utility.h"
#include "colour_detection.h"

#include <opencv2/opencv.hpp>

using cv::Mat;
using cv::createTrackbar;
using std::cout;
using std::endl;

namespace utility {
void showImg(const Mat &img, string title, bool show) {
  if(show) cv::imshow(title.c_str(), img);
}

void showImgWait(const Mat &img, string title) {
  cv::imshow(title.c_str(), img);
  cv::waitKey(0);
}

void createHSVTrackbars(hsvVals &hsv_min, hsvVals &hsv_max) {
  cv::namedWindow("Trackbars");
  cv::resizeWindow("Trackbars", 600, 600);
  createTrackbar("Hue Min", "Trackbars", &hsv_min.hue, utility::MAX_HSV.hue);
  createTrackbar("Hue Max", "Trackbars", &hsv_max.hue, utility::MAX_HSV.hue);
  createTrackbar("Sat Min", "Trackbars", &hsv_min.sat, utility::MAX_HSV.sat);
  createTrackbar("Sat Max", "Trackbars", &hsv_max.sat, utility::MAX_HSV.sat);
  createTrackbar("Val Min", "Trackbars", &hsv_min.val, utility::MAX_HSV.val);
  createTrackbar("Val Max", "Trackbars", &hsv_max.val, utility::MAX_HSV.val);
}

bool inBetween(cv::Point q, cv::Point p1, cv::Point p2) {
  return (std::min(p1.x, p2.x) <= q.x && q.x <= std::max(p1.x, p2.x)) && (std::min(p1.y, p2.y) <= q.y && q.y <= std::max(p1.y, p2.y));;
}

bool inColourRange(cv::Vec3b colour, cv::Vec3b low, cv::Vec3b high) {
  for(int i=0; i<3; ++i) {
    if(!(low[i] <= colour[i] && colour[i] <= high[i])) return false;
  }
  return true;
}
}  // namespace utility