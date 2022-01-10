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

Scalar Pen::chooseColour(Scalar colour) {
  if(colourStyle == CUSTOM) return colour;
  else return {static_cast<double>(rand()%255), static_cast<double>(rand()%255), static_cast<double>(rand()%255)};
}

void Pen::drawPointsOnScreen(Mat &img) {
  for(const auto &obj : trackedObjects) {
    for(const auto &point : obj.points) {
      cv::circle(img, point.p, 5, point.colour, cv::FILLED);
    }
  }
}

void Pen::drawLinesOnScreen(Mat &img) {
  for(const auto &obj : trackedObjects) {
    for (int i = 1; i < obj.points.size(); ++i) {
      if (obj.points[i].connectToPrev) {
        line(img, obj.points[i - 1].p, obj.points[i].p, obj.points[i].colour, 10);
      }
    }
  }
}

void Pen::draw(Mat &img, Mat &imgHSV, vector<Mat> &masks, bool draw_key, bool prev_key) {
  cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
  for (int i = 0; i < highlighters.size(); i++) {
    masks[i] = (colour_detection::findColour(imgHSV, highlighters[i].first.min, highlighters[i].first.max));
    Point myPoint = contour_detection::drawLargestRectangle(img, masks[i]);
    if (draw_key == 1 && myPoint.x != 0 && myPoint.y != 0)
      trackedObjects[i].points.push_back(drawingPoints{myPoint, chooseColour(highlighters[i].second), prev_key});
  }

  if(outputStyle == OUTPUT_STYLE::POINT) {
    drawPointsOnScreen(img);
  } else if(outputStyle == OUTPUT_STYLE::LINE) {
    drawLinesOnScreen(img);
  }
}

void Pen::go(enum OUTPUT_STYLE outputStyle, enum COLOUR_STYLE colourStyle) {
  this->outputStyle = outputStyle;
  this->colourStyle = colourStyle;

  cv::VideoCapture cap(0);
  Mat img, imgHSV;
  if (cap.isOpened() == false) {
    cout << "cannot open webcam\n";
    exit(1);
  }

  vector<Mat> masks(highlighters.size());
  bool draw_key = 0, prev_key = 0;
  while (true) {
    cap.read(img);

    draw(img, imgHSV, masks, draw_key, prev_key);

    cv::flip(img, img, 1);
    cv::imshow("Image", img);
    int temp_key = cv::waitKey(10);
    prev_key = draw_key;
    if (temp_key == 'm')
      draw_key ^= 1;
    else if (temp_key == 'c') {
      for(auto &obj : trackedObjects) {
        obj.points.clear();
      }
    }
    cout << draw_key << endl;
  }
}

Pen::Pen(const vector<pair<hsvRange, Scalar>> &highlighters) : 
  highlighters{highlighters} {
    for(const auto &highlight : highlighters) {
      trackedObjects.push_back({highlight.second});
    }
  };

}