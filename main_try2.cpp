#include <stdio.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <mutex>
//#include <string>

// Affectiva
#include "Frame.h"
#include "Face.h"
#include "AffdexException.h"
#include "FaceListener.h"
#include "ProcessStatusListener.h"
#include "PhotoDetector.h"
#include "CameraDetector.h"

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv/cv.hpp>

using namespace std;
using namespace cv;
using namespace affdex;

class MyApp : public affdex::FaceListener ,  affdex::ImageListener{
public:
	MyApp(CameraDetector* det, int cameraId) {
		this->detector =det;
		int camId = cameraId;
		int camFPS = 60;

		detector->setCameraId(camId);
		detector->setCameraFPS(camFPS);		
		detector->setFaceListener(this);
		detector->setImageListener(this);
	}

	void onFaceFound( float timestamp, FaceId faceId ){
		std::printf("Face Found faceId:%d\n", faceId);
		mfaceId = faceId;
		
	}
	
	void onFaceLost(float timestamp, FaceId faceId){
		std::printf("Face Lost faceId:%d\n", faceId);
	}
	
	void onImageResults(std::map<FaceId, Face> faces, Frame image){

    std::mutex mMutex;
		if (faces.size()>0){
			std::printf("onImageResults... faces.size():%d\n", faces.size());

			for (std::map<FaceId, Face>::iterator it=faces.begin(); it!=faces.end(); ++it){
				Face face=it->second;
				std::printf("FaceId:%d\n", it->first);

				// Expresiones detectables
				std::printf("\nExpresiones:\n");
				std::printf("Smile:%.2f\t", face.expressions.smile);
				std::printf("niyaniya:%.2f\t", face.expressions.smirk);

				std::printf("joy:%.2f\t", face.emotions.joy);
				std::printf("fear:%.2f\t", face.emotions.fear);
				std::printf("disgust:%.2f\t", face.emotions.disgust);
				std::printf("sadnes:%.2f\t", face.emotions.sadness);

				std::printf("anger:%.2f\n", face.emotions.anger);
				std::printf("surprise:%.2f\t", face.emotions.surprise);
				std::printf("valence:%.2f\t", face.emotions.valence);
				std::printf("contempt:%.2f\n", face.emotions.contempt);

        std::shared_ptr<unsigned char> imgdata = image.getBGRByteArray();
        cv::Mat img = cv::Mat(image.getHeight(), image.getWidth(), CV_8UC3, imgdata.get());

        cv::imshow("title",img);
        cv::waitKey(5);

				std::printf("Fin extraccion de datos de la cara %d\n", it->first);
			}
      cv::destroyAllWindows();

			std::printf("Deteccion %d\n", detection);
			
			detection++;
		
		}
	}

	void onImageCapture(Frame image) {
		//std::printf("onImageCapture :\n");
	}

private:
  CameraDetector* detector;
  
  FaceId mfaceId = -1;
  long int detection=0;
};

void onFaceFound( float timestamp, FaceId faceId );

int main(int argc, char ** argsv)
{
	int cameraId=0;
	
	cout << "Se inicia el programa con " << argc << " parametros\n";
	if(argc>1){
		cameraId=atoi(argsv[1]);
	}
	
	cout << "Id de la camara: " << cameraId << endl;

	CameraDetector detector;
  
  std::string classifierPath="/home/kazumi/download/affdex_sdk/data";
	detector.setClassifierPath(classifierPath);
	
	cout << "Se crea el objeto MyApp..." << endl;
	MyApp* myApp = new MyApp(&detector, cameraId);
	
	detector.setDetectSmile(true);
	detector.setDetectJoy(true);
	detector.setDetectAllExpressions(true);
	detector.setDetectAllEmotions(true);
	detector.setDetectAllEmojis(true);
	detector.setDetectAllAppearances(true);	

	cout << "Se inicia el detector." << endl;	
	cout << "Para terminar el programa use Ctrl+C"  << endl;
	detector.start();	
	while(true){

		sleep(0);
	}
	
	detector.stop();

	return 0;
}
