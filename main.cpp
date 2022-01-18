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
  pen.go(pen::POINT, pen::CUSTOM);
}

void usePointer() {
  pointer::Pointer pointer(utility::GREEN_HL_HSV);
  pointer.go();
}

void useSegmenter() {
  tests::testSegmentVideo();
}

void chooseColour() {
  colour_detection::ColourDetector colourDetector;
  colourDetector.videoColourPicker();
}

int main(int argc, char** argv) {
  usePen();
  return 0;
}