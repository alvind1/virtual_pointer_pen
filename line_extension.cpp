#include "line_extension.h"
#include "utility.h"
#include "contour_detection.h"

#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::vector;
using utility::hsvVals;
using utility::hsvRange;

namespace line_extension {

double findSlope(cv::RotatedRect rr) {
  cv::Point2f p[4];
  rr.points(p);
  double slope = 0;
  double temp1 =  pow(p[0].x - p[1].x, 2) + pow(p[0].y - p[1].y, 2);
  double temp2 =  pow(p[1].x - p[2].x, 2) + pow(p[1].y - p[2].y, 2);
  if(temp1 > temp2) {
    slope = (p[0].y - p[1].y)/(p[0].x - p[1].x);
  } else {
    slope = (p[1].y - p[2].y)/(p[1].x - p[2].x);
  }
  return std::atan(slope);
}

pair<cv::Point2f, cv::Point2f> getLineFromRotatedRect(cv::RotatedRect rr) {
  float length = 1000;
  double slope = findSlope(rr);

  float x = rr.center.x + length*cos(slope);
  float y = rr.center.y + length*sin(slope);
  cv::Point2f p{x, y};

  float x2 = rr.center.x - length*cos(slope);
  float y2 = rr.center.y - length*sin(slope);
  cv::Point2f p2{x2, y2};
  return make_pair(p, p2);
}

void drawLineFromRotatedRect(Mat &img, cv::RotatedRect rr) {
  float length = 1000;
  double slope = findSlope(rr);

  float x = rr.center.x + length*cos(slope);
  float y = rr.center.y + length*sin(slope);
  cv::Point2f p{x, y};

  float x2 = rr.center.x - length*cos(slope);
  float y2 = rr.center.y - length*sin(slope);
  cv::Point2f p2{x2, y2};
  cv::line(img, p2, p, {0, 0, 255}, 10);
}

void extendLine() {
  cv::VideoCapture cap(0);
  Mat img, imgHSV, mask;
  if (cap.isOpened() == false) {
    cout << "cannot open webcam\n";
    exit(1);
  }

  hsvRange hsvColours = utility::GREEN_HL_HSV;
  colour_detection::ColourDetector colourDetector;

  while (true) {
    cap.read(img);
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    mask = colourDetector.findColour(imgHSV, hsvColours.min, hsvColours.max); 
    cv::RotatedRect rr = contour_detection::drawLargestRotatedRectangle(img, mask); 
    drawLineFromRotatedRect(img, rr);
    cv::flip(img, img, 1);
    cv::imshow("Image", img);
    cv::waitKey(1);
  }
}
}  // namespace line_extension