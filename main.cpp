#include <stdio.h>
#include "CameraDetector.h"
#include "ImageListener.h"
#include "PhotoDetector.h"
#include <iostream>
#include <string.h>

#include <opencv2/opencv.hpp>
#define WIDTH 320
#define HEIGHT 240


class Listener: public affdex::ImageListener {
  void onImageResults(std::map<affdex::FaceId,affdex::Face>faces,affdex::Frame image){
    std::cout<<"some"<<faces.size()<<std::endl;
    for(auto pair : faces){
      std::string emotion= "neutral?";
      affdex::Face face = pair.second;
      if(face.emotions.joy>25){
        emotion = "Happy:)";
      }else if(face.emotions.sadness>25){
        emotion="sad:(";
      }
      std::cout<<face.id<<":looks"<<emotion<<std::endl;
    }
  };
  void onImageCapture(affdex::Frame image){};
};

int main(int argc, char ** argsv)
{
  int camId = 0;
  int camFPS = 30;

  cv::VideoCapture cap(0);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
  cv::Mat cv_frame;

  char * pixels = new char [WIDTH*HEIGHT*3];
  //char * pixels = new char [img.columns()*img.rows()*3];

  affdex::PhotoDetector detector;

  affdex::ImageListener * listen = new Listener();
  detector.setImageListener(listen);

  std::string classifierPath="/home/kazumi/download/affdex_sdk/data";
  detector.setClassifierPath(classifierPath);

  //std::shared_ptr<affdex::FaceListener> listen(new Listener());
  //std::shared_ptr<PlottingImageListener> listenPtr(new PlottingImageListener(csvFileStream, draw_display));
  //csvFilestream

  //detector.setFaceListener(listen.get());
  detector.setDetectAllEmotions(true);
  detector.start();
  while(1){
    cap >> cv_frame;

    affdex::Frame frame(cv_frame.cols,cv_frame.rows,pixels,affdex::Frame::COLOR_FORMAT::BGR);
    //affdex::Frame frame(img.columns(),img.rows(),pixels,affdex::Frame::COLOR_FORMAT::BGR);
    detector.process(frame);
  }

  detector.stop();
  delete [] pixels;
  return 0;
}
