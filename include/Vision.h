#pragma once

#include <visp3/gui/vpDisplayD3D.h>
#include <visp3/gui/vpDisplayGDI.h>
#include <visp3/gui/vpDisplayGTK.h>
#include <visp3/gui/vpDisplayOpenCV.h>
#include <visp3/gui/vpDisplayX.h>
#include <visp3/io/vpImageIo.h>
#include <visp3/io/vpVideoReader.h>
#include <visp3/blob/vpDot2.h>
#include <visp3/imgproc/vpImgproc.h>
#include <visp3/me/vpMeEllipse.h>
#include <visp3/me/vpMeEllipse.h>
#include <visp3/core/vpConvert.h>
#include <visp3/core/vpImage.h>
#include <visp3/core/vpImageTools.h>
#include <visp3/core/vpImageFilter.h>
#include <visp3/core/vpGaussianFilter.h>
#include <visp3/core/vpImageFilter.h>
#include <visp3/core/vpImageConvert.h>
#include <visp3/core/vpHistogram.h>


#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include<iostream>
#include <vector>
#include <fstream>


#define initial_point_number 5  // the initial number of points for single tracker

class Vision
{

public:

	// used for yolo detector
	
	//int yolo_count;
	std::vector<int> classIds;
	std::vector<float> scores;
	std::vector<cv::Rect> boxes;
	cv::dnn::DetectionModel model;
	std::vector<std::string> classes;

	// used for traditional single tracking
	cv::Point2f RectPoints[initial_point_number];
	struct SingleDropletContour
	{
		std::vector<cv::Point> innerEdge;
		std::vector<cv::Point> outerEdge;
	};

	// used for traditional multiple tracking
	bool selectroi;
	bool treshold;
	bool YOLOinit;
	bool YOLOdetect;


public:

	// Constructor
	Vision(bool imgType);
	Vision(bool imgType, bool RoiFlag);
	

	// initialize the image window
	void InitializeVideo(const std::string videoname);
	void InitializeVideo(const std::string videoname, double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth);
	void InitProcessedImageDisplay();
	void InitProcessedImageDisplay(double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth);
	void InitTrackerImageDisplay();
	void InitTrackerImageDisplay(double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth);


	// acquire image information
	void AcquireImage();
	bool isVideoEnd();
	double getFrameTime_ms();
	long getFrameIndex();
	

	// display images
	void DisplayImage();  // display the raw images
	void DisplayProcessedImage();  // display the edge results(gray)
	void DisplayTrackerImage();  // display the tracker results(gray)


	// update the flush 
	void Flush();
	void FlushProcessedImageDisplay();
	void FlushTrackerImageDisplay();

	
	// transform images
	void ConvertToProcessed();  // convert raw input images into gray processed image
	void CopyProcessedToTracker();  // copy gray processed image into tracker gray image
	

	// image processing on processedgrayImage
	void stretchContrast();
	void Contrast(double alpha, double beta);
	void autoThreshold();  // VISP threshold
	void adaptiveTresholding();  // OpenCV threshold
	void GaussiaBlurring();
	void GaussianFiltering();

	void vpHistogram();
	void equalizeHistogram();
	void sharpen(float sigma, double weight);
	void gammaCorrection(double gamma);


	// ROI selection
	void SelectROI(double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth);  // for colorImage or grayImage
	void SelectROI(vpImage<unsigned char>& source, double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth);
	// crop processedgrayImage
	void Crop_Processed(double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth);


	// screenshot
	void TakeAScreenShot(const std::string videoname, std::string additionalInfo);
	void AutoScreenShot(std::string file);
	void TakeAScreenShotYolo(const std::string videoname, std::string additionalInfo);


	// edge detection
	void DetectEdges();  // VISP edge detection
	cv::Mat DetectEdgesOpenCV(double GaussianSigmaX, double GaussianSigmaY, double CannyThreshold1, double CannyThreshold2);  // OpenCV edge detection

	
	//////////// 
	// Traditional single tracker  
	////////////

	// VISP blob tracker
	//bool blobtracking(bool init_done);

	// VISP ellipse tracker
	bool ellipseTracking(bool ellipse_init_done);
	// save single ellipse results
	void Vision::SaveEllipseParas(std::string file);

	// OpenCV ellipse detector
	auto FindContoursOfSingleDroplet_AreaBased(cv::Mat edge);
	void DrawEllipseOnImage(cv::RotatedRect RotRect);
	std::tuple<double, double, double> FitEllipse(cv::Mat edge, bool drawEllipse, bool printEllipseInfoOnImage);
	
	
	////////////
	// Traditional multi-object tracker: all operations happen on processedgrayImage
	////////////

	// OpenCV blob detector
	void InitOpenCVBlobDetector(cv::SimpleBlobDetector::Params params);
	std::vector<vpImagePoint>  OpenCVDetectBlobs();

	// click to get initial points
	std::vector<vpImagePoint>  GetUserClickedPoints();

	//Conversions
	vpColVector convert_vpImagePointTo_vpColVector(vpImagePoint point);

	//Trackers: implement in Tracking.cpp
	std::vector<vpImagePoint> TrackMunkres(std::vector<vpImagePoint> detectedPoints);
	
	// draw circle or cross on the detected droplets
	void DisplayCircle(vpImagePoint ip, vpColor color);
	void DisplayCross(vpImagePoint ip, vpColor color);



	////////////
	// Deep learning detector
	////////////
	bool Vision::Yolo(bool yolo_init, std::string ConfigPath, std::string ParameterPath);

	std::vector<cv::Rect> Vision::MultiTrackingYolo(std::string ConfigPath, std::string ParameterPath, bool first_run = false);

	
	
private:

#if defined(VISP_HAVE_GDI)
	vpDisplayGDI* display;  // display the raw video
	vpDisplayGDI* processedImageDisplay;  // display the processed video
	vpDisplayGDI* trackerDisplay;  // display the tracker result
#elif defined(VISP_HAVE_OPENCV)
	vpDisplayOpenCV* display;  // display the raw video
	vpDisplayOpenCV* processedImageDisplay;  // display the processed video
	vpDisplayOpenCV* trackerDisplay;  // display the VispEllipse
#endif

	
	vpVideoReader* videoReader;

	bool isColor; // input color image or gray image
	bool isSelectROI;  // a flag to select ROI


	// video information
	double frameTime_ms;
	long frameIndex;
	const float deg2rad = M_PI / 180;


	// input images
	vpImage<vpRGBa> colorImage;  
	vpImage<unsigned char> grayImage;  


	// test images for multi-droplets
	vpImage<vpRGBa> colorTestImage;
	vpImage<unsigned char> grayTestImage;  


	// result images
	//vpImage<vpRGBa> processedcolorImage;
	vpImage<unsigned char> processedgrayImage;  
	vpImage<unsigned char> trackerGrayImage;  


	// OpenCV images
	cv::Mat grayImageOpenCV_yolo;
	cv::Mat colorImageOpenCV_yolo;
	cv::Mat grayImageOpenCV;


	vpImageTools vpTools;
	vpImage<unsigned char> subsitute; 


	/////////  single tracker  /////////
	// blob tracker
	vpImagePoint germ;
	vpDot2 blob;
	

	// ellipse tracker
	vpMe me;
	vpMeEllipse ellipse;
	

	/////////////  Multi-object tracker  ////////////
	cv::Ptr<cv::SimpleBlobDetector> detector;
};
