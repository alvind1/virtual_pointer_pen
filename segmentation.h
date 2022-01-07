#ifndef __SEGMENTATION_H
#define __SEGMENTATION_H

#include <opencv2/opencv.hpp>

using std::string;
using std::vector;

namespace segmentation { 
  void segmentVideo();
  void showSegmentedImg(const string &);
  cv::Mat segmentImg(cv::Mat &, vector<vector<cv::Point>> &, cv::Mat &);
  void testSegmentImg();
  void segment2();
  void removeBadContours(const vector<vector<cv::Point>> &, vector<int> &, int, int, const cv::Mat &);
}

#endif