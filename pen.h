#ifndef __PEN_H
#define __PEN_H

#include "utility.h"
#include "video_manager.h"
#include "colour_detection.h"
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
  struct drawingPoints {
    Point p;
    Scalar colour;
    bool connectToPrev;
    OUTPUT_STYLE outputStyle;
  };
  struct objectInfo{
    Scalar colour;
    vector<drawingPoints> points;
  };
  enum OUTPUT_STYLE outputStyle = POINT;
  enum COLOUR_STYLE colourStyle = CUSTOM;
  colour_detection::ColourDetector colourDetector;
  video_manager::VideoManager<Pen> videoManager{this};
  bool drawKey = 0, prevKey = 0;
  vector<pair<hsvRange, Scalar>> highlighters;
  vector<objectInfo> trackedObjects;
  Mat imgHSV;
  vector<Mat> masks;
  Scalar chooseColour(Scalar); 
  void drawPointsOnScreen(Mat &);
  void drawLinesOnScreen(Mat &);
  void drawOnScreen(Mat &);
  void draw(Mat &, bool, bool);
public:
  Pen(const vector<pair<hsvRange, Scalar>> &);
  void go(enum OUTPUT_STYLE = POINT, enum COLOUR_STYLE = CUSTOM);
  void handle(Mat &img, int key);
};
}

#endif