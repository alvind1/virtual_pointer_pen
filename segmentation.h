#ifndef __SEGMENTATION_H
#define __SEGMENTATION_H

#include <opencv2/opencv.hpp>

using std::string;
using std::vector;
using cv::Mat;
using cv::Point;

namespace segmentation { 
void showSegmentedImg(const string &);

class Segmenter {
private:
  bool debug = false;
  vector<vector<Point>> contours;
  Mat markers;
  Mat preprocessImg(const Mat &img);
public:
  Mat segmentImg(const Mat &img);
  void removeBadContours(const vector<vector<cv::Point>> &, vector<int> &, int, int);
  void segmentVideo();
  const Mat &getMarkers();
  const vector<vector<Point>> &getContours();
};
}

#endif