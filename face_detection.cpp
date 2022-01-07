#include "face_detection.h"
#include "utility.h"

#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using cv::Mat;
using utility::IMAGE_PATH;
using utility::OPENCV4_PATH;

namespace face_detection {
void detectFaceInImg() {
  string haar_path = OPENCV4_PATH + "haarcascades/haarcascade_frontalface_default.xml";
  cv::CascadeClassifier faceCascade;
  faceCascade.load(haar_path);

  if (faceCascade.empty()) {
    cout << "XML file not loaded" << endl;
    exit(1);
  }

  string path = IMAGE_PATH + "Maui.jpg";
  Mat img = cv::imread(path);
  vector<cv::Rect> faces;
  faceCascade.detectMultiScale(img, faces, 1.1, 10);

  for (int i = 0; i < faces.size(); ++i) {
    rectangle(img, faces[i].tl(), faces[i].br(), cv::Scalar(255, 0, 255), 3);
  }

  cv::imshow("Image", img);
  cv::waitKey(0);
}

void detectFaceInVideo() {
  string haar_path = OPENCV4_PATH + "haarcascades/haarcascade_frontalface_default.xml";
  cv::CascadeClassifier faceCascade;
  faceCascade.load(haar_path);

  if (faceCascade.empty()) {
    cout << "XML file not loaded" << endl;
    exit(1);
  }

  cv::VideoCapture cap(0);
  Mat img;
  vector<cv::Rect> faces;
  while (true) {
    cap.read(img);
    faceCascade.detectMultiScale(img, faces, 1.1, 10);
    for (int i = 0; i < faces.size(); ++i) {
      rectangle(img, faces[i].tl(), faces[i].br(), cv::Scalar(255, 0, 255), 3);
    }

    cv::imshow("Image", img);
    cv::waitKey(1);
  }
}
}  // namespace face_detection