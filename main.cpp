#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "colour_detection.h"
#include "contour_detection.h"
#include "demos.h"
#include "face_detection.h"
#include "line_extension.h"
#include "segmentation.h"
#include "pen.h"
#include "utility.h"

using utility::IMAGE_PATH;

void test() {
  // Image colour detection
  //colour_detection::imageColourDetection(utility::IMAGE_PATH + "Tesla.jpg");

  // Image Colour Picker
  //colour_detection::imageColourPicker(cv::imread(IMAGE_PATH + "Tesla.jpg"));

  // Webcam colour detection
  //colour_detection::videoColourPicker();

  // Contour detection
  //demos::shapeDetection();

  // Contour detection in video
  //contour_detection::detectContoursFromVideo();

  // Pen
  //pen::pen();

  //face_detection::detectFaceInImg();
  //face_detection::detectFaceInVideo();

  //segmentation::showSegmentedImg(utility::IMAGE_PATH + "mydesk.jpg");
  //segmentation::testSegmentImg();
  //segmentation::segment2();

  //segmentation::segmentVideo();

  //line_extension::extendLine();

  //demos::highlightClosestContour();

  //demos::testHighlightClosestContourToLine();

  demos::big();
}

int main(int argc, char** argv) {
  test();
  return 0;
}