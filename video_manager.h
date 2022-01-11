#ifndef __VIDEO_MANAGER
#define __VIDEO_MANAGER

#include <opencv2/opencv.hpp>

using std::cout;
using std::endl;
using cv::Mat;
using cv::VideoCapture;

namespace video_manager {

template<typename T>
class VideoManager {
private:
  T *obj;
public:
  void go() {
    Mat img;
    int key = 0;
    cv::VideoCapture cap(0);
    if (cap.isOpened() == false) {
      cout << "cannot open webcam\n";
      exit(1);
    }
    while (true) {
      cap.read(img);

      obj->handle(img, key);

      cv::flip(img, img, 1);
      cv::imshow("Image", img);
      key = cv::waitKey(1);
    }
  }
  VideoManager(T *obj) : obj{obj} {}
};
}

#endif