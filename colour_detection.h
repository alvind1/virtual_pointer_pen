#ifndef COLOUR_DETECTION_H
#define COLOUR_DETECTION_H
#include <opencv2/opencv.hpp>
#include <string>

#include "utility.h"

using std::string;
using utility::hsvVals;
using cv::Mat;

namespace colour_detection {
Mat convertImageHSV(const Mat &);
Mat findColour(const Mat &img, const hsvVals min, const hsvVals max);
void imageColourDetection(string);
void imageColourPicker(const Mat &);
void videoColourPicker();
}  // namespace colour_detection
#endif