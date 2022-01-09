#ifndef __CONTOUR_DETECTION
#define __CONTOUR_DETECTION
#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat;
using std::vector;

namespace contour_detection {
void getContouredImg(const Mat &, Mat &, bool);
vector<vector<cv::Point>> getContours(const Mat &);
void outlineContours(Mat &, const Mat &, int areaThreshold = -1);
void outlineShapes(Mat, const Mat &, int areaThreshold = -1);

vector<cv::Point> findLargestRectangle(const Mat &);
cv::RotatedRect drawRotatedRectangle(Mat &, const vector<cv::Point> &);
cv::RotatedRect drawRotatedRectangle(Mat &, const cv::RotatedRect &);
cv::Point drawLargestRectangle(Mat, const Mat &);
cv::RotatedRect drawLargestRotatedRectangle(Mat, const Mat &);

void detectContoursFromImg(Mat &, int);
void detectContoursFromVideo();
void detectShapesFromImg(Mat &);
void shapeDetection();
}  // namespace contour_detection
#endif