#include <stdio.h>
#include "CameraDetector.h"
#include <iostream>

class Listener: public affdex::FaceListener {
  void onFaceFound(float timestamp, affdex::FaceId faceId){
    std::cout<<"Hi,"<<faceId<<std::endl;
  }
  void onFaceLost(float timestamp, affdex::FaceId faceId){
    std::cout << "Bye," <<faceId<<std::endl;
  }
};

int main(int argc, char ** argsv)
{
  int camId = 0;
  int camFPS = 30;

  affdex::CameraDetector detector;
  std::string classifierPath="/home/kazumi/download/affdex_sdk/data";
  detector.setClassifierPath(classifierPath);

  //detector.setCameraId(camId);
  //detector.setCameraFPS(camFPS);

  detector.setDetectSmile(true);
  detector.setDetectAllEmotions(true);

  std::shared_ptr<affdex::FaceListener> listen(new Listener());
  detector.setFaceListener(listen.get());

  detector.start();

  detector.getDetectSmile();

  int x;
  std::cin >> x;

  detector.stop();
  
  return 0;
}
