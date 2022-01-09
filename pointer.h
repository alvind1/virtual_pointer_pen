#ifndef DEMOS
#define DEMOS
#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat;
using cv::Point;
using std::string;
using std::vector;

namespace pointer {
void highlightClosestContour();
void pointer();
void highlightClosestContourToLine(Mat &img, cv::Point2f, cv::Point2f, bool showLine = true);
void testHighlightClosestContourToLine(); 
}  // namespace demos
#endif