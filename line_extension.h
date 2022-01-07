#ifndef LINE_EXT
#define LINE_EXT
#include <opencv2/opencv.hpp>

#include "colour_detection.h"
namespace line_extension {
void extendLine();
void drawLineFromRotatedRect(Mat &, cv::RotatedRect);
pair<cv::Point2f, cv::Point2f> getLineFromRotatedRect(cv::RotatedRect);
}

#endif