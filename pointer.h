#ifndef DEMOS
#define DEMOS
#include <opencv2/opencv.hpp>
#include <vector>

#include "segmentation.h"
#include "video_manager.h"
#include "utility.h"

using cv::Mat;
using cv::Point;
using std::string;
using std::vector;
using std::pair;

namespace pointer {
class Pointer {
private:
  video_manager::VideoManager<Pointer> videoManager{this};
  utility::hsvRange hsvColours;
  segmentation::Segmenter segmenter;
  pair<double, int> findClosestContourToPoint(const vector<vector<cv::Point>> &, const vector<int> &, cv::Point);
  void highlightClosestContour();
  void highlightClosestContourToLine(Mat &, cv::Point2f, cv::Point2f, bool = true);
  void testHighlightClosestContourToLine();
public:
  Pointer(utility::hsvRange);
  void point(Mat &img);
  void handle(Mat &img, int key);
  void go();
};
}  // namespace demos
#endif