#ifndef __PEN_H
#define __PEN_H

#include "utility.h"
#include <opencv2/opencv.hpp>

using std::vector;
using std::pair;
using cv::Scalar;
using cv::Mat;
using utility::hsvRange;

namespace pen {
class Pen {
private:
  vector<pair<hsvRange, Scalar>> highlighters;
  struct drawingPoints;
  void drawPointsOnScreen(Mat, vector<drawingPoints>);
  void drawLinesOnScreen(Mat, vector<drawingPoints>, vector<Scalar> colours = {{255, 0, 0}, {0, 255, 0}});
  void draw(Mat &, Mat &, vector<drawingPoints> &, vector<Mat> &, bool, bool);
public:
  Pen(const vector<pair<hsvRange, Scalar>> &);
  void go();
};
}

#endif