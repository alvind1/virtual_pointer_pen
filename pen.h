#ifndef __PEN_H
#define __PEN_H

#include "utility.h"
#include <opencv2/opencv.hpp>

using std::vector;
using std::pair;
using cv::Scalar;
using cv::Mat;
using cv::Point;
using utility::hsvRange;

namespace pen {
enum OUTPUT_STYLE {
  POINT, LINE
};

enum COLOUR_STYLE {
  CUSTOM, RANDOM
};

class Pen {
private:
  vector<pair<hsvRange, Scalar>> highlighters;
  enum OUTPUT_STYLE outputStyle = POINT;
  enum COLOUR_STYLE colourStyle = CUSTOM;
  struct drawingPoints {
    Point p;
    Scalar colour;
    bool connectToPrev;
  };
  struct objectInfo{
    Scalar colour;
    vector<drawingPoints> points;
  };
  vector<objectInfo> trackedObjects;
  Scalar chooseColour(Scalar); 
  void drawPointsOnScreen(Mat &);
  void drawLinesOnScreen(Mat &);
  void draw(Mat &, Mat &, vector<Mat> &, bool, bool);
public:
  Pen(const vector<pair<hsvRange, Scalar>> &);
  void go(enum OUTPUT_STYLE = POINT, enum COLOUR_STYLE = CUSTOM);
};
}

#endif