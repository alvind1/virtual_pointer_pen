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
  vector<pair<hsvRange, Scalar>> highlighters;
  for(size_t i=0; i<hsvColours.size(); ++i) {
    highlighters.push_back(make_pair(hsvColours[i], utility::COLOUR_LIST[i]));
  }
  pen::Pen pen(highlighters);
  pen.go(pen::LINE, pen::CUSTOM);
}

void usePointer() {
  pointer::Pointer pointer;
  pointer.point();
}

void useSegmenter() {
  tests::testSegmentVideo();
}

int main(int argc, char** argv) {
  useSegmenter();
  return 0;
}