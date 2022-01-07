#include "demos.h"

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "colour_detection.h"
#include "contour_detection.h"
#include "segmentation.h"
#include "line_extension.h"
#include "utility.h"

using std::cout;
using std::endl;
using utility::IMAGE_PATH;

namespace demos {

void shapeDetection() {
  Mat img = cv::imread(IMAGE_PATH + "Shapes.jpg");
  contour_detection::detectShapesFromImg(img);
  utility::showImgWait(img);
}

pair<double, int> findClosestContourToPoint(const vector<vector<cv::Point>> &contours, const vector<int> &indices, cv::Point point) {
  double minDist = 10000;
  int minIndex = 0;
  for(auto it : indices) {
    double tempDist = abs(cv::pointPolygonTest(contours[it-1], point, true));
    if(tempDist < minDist) {
      minDist = tempDist;
      minIndex = it;
    }
  }
  return make_pair(minDist, minIndex);
}

void testHighlightClosestContourToLine() {
  Mat img = cv::imread(utility::IMAGE_PATH+"mydesk.jpg");
  float pointy = static_cast<float> (rand()%img.rows);
  float pointx = static_cast<float>(rand()%img.cols);
  cout << pointx << " " << pointy << endl;

  float pointy2 = static_cast<float> (rand()%img.rows);
  float pointx2 = static_cast<float>(rand()%img.cols);
  highlightClosestContourToLine(img, cv::Point2f{pointx, pointy}, cv::Point2f{pointx2, pointy2});
  utility::showImgWait(img);
}

void highlightClosestContourToLine(Mat &img, cv::Point2f p1, cv::Point2f p2, bool showLine) {
  vector<vector<cv::Point>> contours;
  Mat markers;
  segmentation::segmentImg(img, contours, markers);
  vector<int> indices(contours.size(), 0);
  for(int i=1; i<=contours.size(); ++i) indices[i-1] = i;

  vector<vector<cv::Point>> contours2;
  for(auto index : indices) {
    Mat temp = (markers==index);
    contours2.push_back(contour_detection::getContours(temp)[0]);
  }
  segmentation::removeBadContours(contours2, indices, img.rows, img.cols, img);

  if(showLine) cv::line(img, p1, p2, {0, 0, 255}, 10);

  double minDist = 100000;
  int minIndex;

  int steps = 10;
  float stepX = (p2.x - p1.x)/steps;
  float stepY = (p2.y - p1.y)/steps;
  
  for(cv::Point2f pt{p1.x, p1.y}; utility::inBetween(pt, p1, p2); pt.x += stepX, pt.y += stepY) {
    pair<double, int> minMask = findClosestContourToPoint(contours2, indices, pt);
    if(minMask.first < minDist) {
      minDist = minMask.first;
      minIndex = minMask.second;
    }
  }
  cout << "line points " << p1 << " " << p2 << endl;
  cout << "MIN " << minDist << " " << minIndex << endl;

  Mat mask = (markers == minIndex);
  contour_detection::outlineContours(img, mask);
}

void highlightClosestContour() {
  Mat img = cv::imread(IMAGE_PATH + "mydesk.jpg");
  vector<vector<cv::Point>> contours;
  Mat markers;
  segmentation::segmentImg(img, contours, markers);
  vector<int> indices(contours.size(), 0);
  for(int i=1; i<=contours.size(); ++i) indices[i-1] = i;

  vector<vector<cv::Point>> contours2;
  for(auto index : indices) {
    Mat temp = (markers==index);
    contours2.push_back(contour_detection::getContours(temp)[0]);
  }
  segmentation::removeBadContours(contours2, indices, img.rows, img.cols, img);

  float pointy = static_cast<float> (rand()%img.rows);
  float pointx = static_cast<float>(rand()%img.cols);
  cout << pointx << " " << pointy << endl;
  cv::Point2f circ = {pointx, pointy};
  cv::circle(img, circ, 5, {0, 0, 255});

  pair<double, int> minMask = findClosestContourToPoint(contours2, indices, circ);

  Mat mask = (markers == minMask.second);
  contour_detection::outlineContours(img, mask);
  utility::showImgWait(img);
}

void big() {
  cv::VideoCapture cap(0);
  Mat img, imgHSV, mask;
  if (cap.isOpened() == false) {
    cout << "cannot open webcam\n";
    exit(1);
  }

  pair<hsvVals, hsvVals> hsvColours = utility::GREEN_HL_HSV;

  vector<vector<Point>> contours;
  Mat markers;

  while (true) {
    cap.read(img);
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    mask = colour_detection::findColour(imgHSV, hsvColours.first, hsvColours.second); 
    vector<Point> contour = contour_detection::findLargestRectangle(mask);
    cv::RotatedRect rr1;
    if(contour.size() != 0) rr1 = cv::minAreaRect(contour);
    pair<cv::Point2f, cv::Point2f> ln = line_extension::getLineFromRotatedRect(rr1);

    if(!isnan(ln.first.x) && !isnan(ln.first.y) && !isnan(ln.second.x) && !isnan(ln.second.y)) {
      highlightClosestContourToLine(img, ln.first, ln.second, false);
    }
    cv::flip(img, img, 1);
    cv::imshow("Image", img);
    cv::waitKey(1);
  }
}

}  // namespace demos