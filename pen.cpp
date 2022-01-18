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

void Pen::drawOnScreen(Mat &img) {
  for(const auto &obj : trackedObjects) {
    for (int i = 0; i < obj.points.size(); ++i) {
      if(obj.points[i].outputStyle == POINT) {
        cv::circle(img, obj.points[i].p, 5, obj.points[i].colour, cv::FILLED);
      } else if(i > 0 && obj.points[i-1].outputStyle == LINE) {
        if(obj.points[i].connectToPrev) {
          line(img, obj.points[i - 1].p, obj.points[i].p, obj.points[i].colour, 10);
        }
      }
    }
  }
}

void Pen::draw(Mat &img, bool draw_key, bool prev_key) {
  cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
  for (int i = 0; i < highlighters.size(); i++) {
    masks[i] = (colourDetector.findColour(imgHSV, highlighters[i].first.min, highlighters[i].first.max));
    Point myPoint = contour_detection::drawLargestRectangle(img, masks[i]);
    if (draw_key == 1 && myPoint.x != 0 && myPoint.y != 0)
      trackedObjects[i].points.push_back(drawingPoints{myPoint, chooseColour(highlighters[i].second), prev_key, outputStyle});
  }

  //if(outputStyle == OUTPUT_STYLE::POINT) {
  //  drawPointsOnScreen(img);
  //} else if(outputStyle == OUTPUT_STYLE::LINE) {
  //  drawLinesOnScreen(img);
  //}

  drawOnScreen(img);
}

void Pen::handle(Mat &img, int key) {
  prevKey = drawKey;
  if (key == 'm')
    drawKey ^= 1;
  else if (key == 'c') {
    for(auto &obj : trackedObjects) {
      obj.points.clear();
    }
  } else if(key == 't') {
    if(outputStyle == POINT) outputStyle = LINE;
    else if(outputStyle == LINE) outputStyle = POINT;
  }
  cout << drawKey << endl;

  draw(img, drawKey, prevKey);
}

void Pen::go(enum OUTPUT_STYLE outputStyle, enum COLOUR_STYLE colourStyle) {
  this->outputStyle = outputStyle;
  this->colourStyle = colourStyle;

  videoManager.go();
}

Pen::Pen(const vector<pair<hsvRange, Scalar>> &highlighters) : 
  highlighters{highlighters}, masks(highlighters.size()) {
  for(const auto &highlight : highlighters) {
    trackedObjects.push_back({highlight.second});
  }
};

}