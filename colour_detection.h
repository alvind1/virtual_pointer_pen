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
void imageColourDetection(string);

class ColourDetector {
private:
  Mat imgHSV;
  Mat mask;
public:
  Mat convertImageHSV(const Mat &);
  Mat findColour(const Mat &, const hsvVals, const hsvVals); 
  void imageColourPicker(const Mat &);
  void videoColourPicker();
  void imageColourDetection(string path); 
  const Mat &getImgHSV();
};
}  // namespace colour_detection
#endif