#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "colour_detection.h"
#include "contour_detection.h"
#include "pointer.h"
#include "face_detection.h"
#include "line_extension.h"
#include "segmentation.h"
#include "pen.h"
#include "utility.h"
#include "tests.h"

using std::make_pair;
using utility::hsvColours;
using utility::hsvRange;

void usePen() {
  vector<pair<hsvRange, Scalar>> colours;
  for(auto colour : hsvColours) {
    colours.push_back(make_pair(colour, cv::Scalar{255, 0, 0}));
  }
  pen::Pen pen(colours);
  pen.go(pen::LINE);
}

void usePointer() {
  pointer::Pointer pointer;
  pointer.point();
}

int main(int argc, char** argv) {
  usePen();
  return 0;
}