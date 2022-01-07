#ifndef UTILITY_H
#define UTILITY_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "configurations.h"

#define PI 3.1415926
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

namespace utility {
struct hsvVals {
  int hue, sat, val;
};
const hsvVals MIN_HSV{0, 0, 0};
const hsvVals MAX_HSV{179, 255, 255};  // hue, saturation, value
const pair<hsvVals, hsvVals> GREEN_HSV = make_pair(hsvVals{46, 37, 83}, hsvVals{70, 119, 255});  // Green marker
const pair<hsvVals, hsvVals> BLUE_HSV = make_pair(hsvVals{83, 74, 15}, hsvVals{133, 182, 242});  // Blue marker

const pair<hsvVals, hsvVals> GREEN_HL_HSV = make_pair(hsvVals{71, 52, 0}, hsvVals{124, 255, 243});  // Green Highlighter

const pair<hsvVals, hsvVals> YELLOW_HL_HSV = make_pair( hsvVals{23, 74, 177}, hsvVals{56, 127, 255});  // Yellow Highlighter
const pair<hsvVals, hsvVals> RED_PEN_HSV = make_pair(hsvVals{0, 137, 184}, hsvVals{22, 173, 212});  // Red Pen
const pair<hsvVals, hsvVals> BLUE_PEN_HSV = make_pair(hsvVals{36, 43, 56}, hsvVals{132, 158, 188});  // Blue Pen
const vector<pair<hsvVals, hsvVals>> hsv_colours{RED_PEN_HSV, BLUE_PEN_HSV};

const vector<cv::Vec3b> COLOUR_LIST{{255, 0, 0},   {0, 0, 255},
                                    {0, 255, 0},   {255, 255, 0},
                                    {255, 0, 255}, {0, 255, 255}};

using configs::IMAGE_PATH;
using configs::OPENCV4_PATH;

void show_img(const cv::Mat &, string title = "Image");
void showImgWait(const cv::Mat &, string title = "Image");
void createHSVTrackbars(hsvVals &, hsvVals &);

bool inBetween(cv::Point, cv::Point, cv::Point);
bool inColourRange(cv::Vec3b, cv::Vec3b, cv::Vec3b);
}  // namespace utility

#endif