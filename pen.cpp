#include "pen.h"
#include "contour_detection.h"
#include "colour_detection.h"
#include "utility.h"

#include <opencv2/opencv.hpp>

using std::vector;
using std::cout;
using std::endl;
using cv::Mat;
using cv::Point;
using cv::Scalar;

namespace pen {
struct Pen::drawingPoints {
  Point p;
  Scalar colour;
  bool connectToPrev;
};

void Pen::drawPointsOnScreen(Mat img, vector<drawingPoints> newPoints) {
  for (int i = 0; i < newPoints.size(); ++i) {
    cv::circle(img, newPoints[i].p, 5, newPoints[i].colour, cv::FILLED);
  }
}

void Pen::drawLinesOnScreen(Mat img, vector<drawingPoints> newPoints) {
  for (int i = 1; i < newPoints.size(); ++i) {
    if (newPoints[i - 1].colour == newPoints[i].colour && newPoints[i].connectToPrev) {
      line(img, newPoints[i - 1].p, newPoints[i].p, newPoints[i].colour, 10);
    }
  }
}

void Pen::draw(Mat &img, Mat &imgHSV, vector<drawingPoints> &newPoints, vector<Mat> &masks, bool draw_key, bool prev_key) {
  cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
  for (int i = 0; i < highlighters.size(); i++) {
    masks[i] = (colour_detection::findColour(imgHSV, highlighters[i].first.min, highlighters[i].first.max));
    Point myPoint = contour_detection::drawLargestRectangle(img, masks[i]);
    if (draw_key == 1 && myPoint.x != 0 && myPoint.y != 0)
      newPoints.push_back(drawingPoints{myPoint, highlighters[i].second, prev_key});
  }

  if(style == STYLE::POINT) {
    drawPointsOnScreen(img, newPoints);
  } else if(style == STYLE::LINE) {
    drawLinesOnScreen(img, newPoints);
  }
}

void Pen::go(enum STYLE style) {
  this->style = style;
  cv::VideoCapture cap(0);
  Mat img, imgHSV;
  if (cap.isOpened() == false) {
    cout << "cannot open webcam\n";
    exit(1);
  }

  vector<drawingPoints> newPoints;
  vector<Mat> masks(highlighters.size());
  bool draw_key = 0, prev_key = 0;
  while (true) {
    cap.read(img);

    draw(img, imgHSV, newPoints, masks, draw_key, prev_key);

    cv::flip(img, img, 1);
    cv::imshow("Image", img);
    int temp_key = cv::waitKey(10);
    prev_key = draw_key;
    if (temp_key == 'm')
      draw_key ^= 1;
    else if (temp_key == 'c')
      newPoints.clear();
    cout << draw_key << endl;
  }
}

Pen::Pen(const vector<pair<hsvRange, Scalar>> &highlighters) : highlighters{highlighters} {};

}