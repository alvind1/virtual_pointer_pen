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
using utility::hsv_colours;

namespace pen {

struct drawingPoints {
  Point p;
  int c;
  bool connect_to_prev;
};

void drawPointsOnScreen(Mat img, vector<drawingPoints> newPoints, vector<Scalar> colours = {{255, 0, 0}, {0, 255, 0}}) {
  for (int i = 0; i < newPoints.size(); ++i) {
    cv::circle(img, newPoints[i].p, 5, colours[newPoints[i].c], cv::FILLED);
  }
}

void drawLinesOnScreen(Mat img, vector<drawingPoints> newPoints, vector<Scalar> colours = {{255, 0, 0}, {0, 255, 0}}) {
  for (int i = 1; i < newPoints.size(); ++i) {
    if (newPoints[i - 1].c == newPoints[i].c && newPoints[i].connect_to_prev) {
      line(img, newPoints[i - 1].p, newPoints[i].p, colours[newPoints[i].c], 10);
    }
  }
}

void pen() {
  cv::VideoCapture cap(0);
  Mat img, imgHSV;
  if (cap.isOpened() == false) {
    cout << "cannot open webcam\n";
    exit(1);
  }

  vector<drawingPoints> newPoints;
  vector<Mat> masks(hsv_colours.size());
  bool draw_key = 0, prev_key = 0;
  while (true) {
    cap.read(img);
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    for (int i = 0; i < hsv_colours.size(); i++) {
      masks[i] = (colour_detection::findColour(imgHSV, hsv_colours[i].first, hsv_colours[i].second));
      Point myPoint = contour_detection::drawLargestRectangle(img, masks[i]);
      if (draw_key == 1 && myPoint.x != 0 && myPoint.y != 0)
        newPoints.push_back(drawingPoints{myPoint, i, prev_key});
    }

    drawPointsOnScreen(img, newPoints);
    // drawLinesOnScreen(img, newPoints);
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

}