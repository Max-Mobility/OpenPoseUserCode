#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <string>
#include "opencv2/opencv.hpp"

//debug

#include <iostream>

class FaceDetector
{

public:

  FaceDetector()
  {
	_loaded = _faceClassifier.load(_faceCascadeName);
	_loaded &= _eyeClassifier.load(_eyeCascadeName);
  }

  FaceDetector(std::string faceCascadeName, std::string eyeCascadeName, double scale = 1, bool tryFlip = true) :
	_faceCascadeName(faceCascadeName), _eyeCascadeName(eyeCascadeName), _scale(scale), _tryFlip(tryFlip)
  {
	_loaded = _faceClassifier.load(_faceCascadeName);
	_loaded &= _eyeClassifier.load(_eyeCascadeName);
  }

  bool isLoaded()
  {
	return _loaded;
  }

  void detect(cv::Mat& img)
  {
	std::vector<cv::Rect> faces, faces2;
    const static cv::Scalar colors[] =
	  {
        cv::Scalar(255,0,0),
        cv::Scalar(255,128,0),
        cv::Scalar(255,255,0),
        cv::Scalar(0,255,0),
        cv::Scalar(0,128,255),
        cv::Scalar(0,255,255),
        cv::Scalar(0,0,255),
        cv::Scalar(255,0,255)
	  };
	cv::Mat gray, smallImg;
	cv::cvtColor( img, gray, cv::COLOR_BGR2GRAY );
    double fx = 1 / _scale;
	cv::resize( gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR_EXACT );
	cv::equalizeHist( smallImg, smallImg );
    _faceClassifier.detectMultiScale( smallImg, faces,
									  1.1, 2, 0
									  //|CASCADE_FIND_BIGGEST_OBJECT
									  //|CASCADE_DO_ROUGH_SEARCH
									  |cv::CASCADE_SCALE_IMAGE,
									  cv::Size(30, 30) );
    if( _tryFlip )
	  {
        cv::flip(smallImg, smallImg, 1);
        _faceClassifier.detectMultiScale( smallImg, faces2,
										  1.1, 2, 0
										  //|CASCADE_FIND_BIGGEST_OBJECT
										  //|CASCADE_DO_ROUGH_SEARCH
										  |cv::CASCADE_SCALE_IMAGE,
										  cv::Size(30, 30) );
        for( auto& r : faces2 )
		  {
            faces.push_back(cv::Rect(smallImg.cols - r.x - r.width, r.y, r.width, r.height));
		  }
	  }
    for ( size_t i = 0; i < faces.size(); i++ )
	  {
        cv::Rect r = faces[i];
        cv::Mat smallImgROI;
		std::vector<cv::Rect> nestedObjects;
        cv::Point center;
        cv::Scalar color = colors[i%8];
        int radius;
        double aspect_ratio = (double)r.width/r.height;
		/*        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
				  {
				  center.x = cvRound((r.x + r.width*0.5)*_scale);
				  center.y = cvRound((r.y + r.height*0.5)*_scale);
				  radius = cvRound((r.width + r.height)*0.25*_scale);
				  cv::circle( img, center, radius, color, 3, 8, 0 );
				  }
				  else*/
		cv::rectangle( img, cvPoint(cvRound(r.x*_scale), cvRound(r.y*_scale)),
					   cvPoint(cvRound((r.x + r.width-1)*_scale), cvRound((r.y + r.height-1)*_scale)),
					   color, 3, 8, 0);
        smallImgROI = smallImg( r );
        _eyeClassifier.detectMultiScale( smallImgROI, nestedObjects,
										 1.1, 2, 0
										 //|CASCADE_FIND_BIGGEST_OBJECT
										 //|CASCADE_DO_ROUGH_SEARCH
										 //|CASCADE_DO_CANNY_PRUNING
										 |cv::CASCADE_SCALE_IMAGE,
										 cv::Size(30, 30) );
        for ( size_t j = 0; j < nestedObjects.size(); j++ )
		  {
            cv::Rect nr = nestedObjects[j];
            /*center.x = cvRound((r.x + nr.x + nr.width*0.5)*_scale);
            center.y = cvRound((r.y + nr.y + nr.height*0.5)*_scale);
            radius = cvRound((nr.width + nr.height)*0.25*_scale);
            cv::circle( img, center, radius, color, 3, 8, 0 );*/
			cv::rectangle(img, cvPoint(cvRound((r.x + nr.x)*_scale), cvRound((r.y + nr.y)*_scale)),
						  cvPoint(cvRound((r.x + nr.x + nr.width-1)*_scale), cvRound((r.y + nr.y + nr.height-1)*_scale)),
						  color, 3, 8, 0);

		  }
	  }
  };

private:

  //classifier objects
  cv::CascadeClassifier _faceClassifier;
  cv::CascadeClassifier _eyeClassifier;

  //classifer xml filenames
  std::string _faceCascadeName = "/home/jeb/repositories/openpose/examples/user_code/haarcascades/haarcascade_frontalface_alt.xml";
  std::string _eyeCascadeName = "/home/jeb/repositories/openpose/examples/user_code/haarcascades/haarcascade_eye.xml";

  //classificationparameters
  double _scale = 1;
  bool _tryFlip = false;

  //classification output
  std::vector<cv::Rect> _faces;
  std::vector<cv::Rect> _facesFlipped;
  std::vector<cv::Rect> _eyes;

  //status
  bool _loaded = false;






};

#endif
