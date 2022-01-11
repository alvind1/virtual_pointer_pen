#ifndef DEMOS
#define DEMOS
#include <opencv2/opencv.hpp>
#include <vector>

#include "segmentation.h"

using cv::Mat;
using cv::Point;
using std::string;
using std::vector;
using std::pair;

namespace pointer {
class Pointer {
private:
  segmentation::Segmenter segmenter;
  pair<double, int> findClosestContourToPoint(const vector<vector<cv::Point>> &, const vector<int> &, cv::Point);
  void highlightClosestContour();
  void highlightClosestContourToLine(Mat &, cv::Point2f, cv::Point2f, bool = true);
  void testHighlightClosestContourToLine();
public:
  void point();
};
}  // namespace demos
#endif