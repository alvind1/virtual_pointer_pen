#ifndef DEMOS
#define DEMOS
#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat;
using cv::Point;
using std::string;
using std::vector;
using std::pair;

namespace pointer {
class Pointer {
private:
  pair<double, int> findClosestContourToPoint(const vector<vector<cv::Point>> &, const vector<int> &, cv::Point); 
  void highlightClosestContour();
  void highlightClosestContourToLine(Mat &, cv::Point2f, cv::Point2f, bool showLine = true);
  void testHighlightClosestContourToLine(); 
public:
  void point();
};
}  // namespace demos
#endif