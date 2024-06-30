#include<Vision.h>
#include <iostream>      
#include <string> 
#include "MatlabExport.h"
// Default constructor
//Vision::Vision() {};

using namespace std;
using namespace cv;
using namespace dnn;

// the droplet position using single ellipse tracker
static int ellipse_times = 0;
std::vector<std::vector<double> > ellipse_paras(0, std::vector<double>(5));



bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2);


Vision::Vision(bool imgType)
{

#if defined(VISP_HAVE_GDI)
	display = new vpDisplayGDI();
	processedImageDisplay = new vpDisplayGDI();
	trackerDisplay = new vpDisplayGDI();
#elif defined(VISP_HAVE_OPENCV)
	display = new vpDisplayOpenCV();
	processedImageDisplay = new vpDisplayOpenCV();
	trackerDisplay = new vpDisplayOpenCV();
#endif

	isColor = imgType;
	isSelectROI = false;

	videoReader = new vpVideoReader();

	frameTime_ms = -1;
	frameIndex = -1;

}

Vision::Vision(bool imgType, bool RoiFlag)
{
	
#if defined(VISP_HAVE_GDI)
	display = new vpDisplayGDI();
	processedImageDisplay = new vpDisplayGDI();
	trackerDisplay = new vpDisplayGDI();
#elif defined(VISP_HAVE_OPENCV)
	display = new vpDisplayOpenCV();
	processedImageDisplay = new vpDisplayOpenCV();
	trackerDisplay = new vpDisplayOpenCV();
#endif

	isColor = imgType;
	isSelectROI = RoiFlag;

	videoReader = new vpVideoReader();

	frameTime_ms = -1;
	frameIndex = -1;
}


void Vision::InitializeVideo(const std::string videoname)
{
	videoReader->setFileName(videoname);
	int videoWidth;
	int videoHeight;
	if (isColor)  // color images
	{
		videoReader->open(colorImage);
		videoWidth = colorImage.getWidth();
		videoHeight = colorImage.getHeight();

	}
	else  // gray images
	{
		videoReader->open(grayImage);
		videoWidth = grayImage.getWidth();
		videoHeight = grayImage.getHeight();
	}
	double videoFPS = videoReader->getFramerate();
	frameTime_ms = 1.0 / videoFPS * 1000.0;
	std::cout << "Video name     : " << videoname << std::endl;
	std::cout << "Video framerate: " << videoFPS << "fps" << std::endl;
	std::cout << "Video dimension: " << videoWidth << " " << videoHeight << std::endl;
	//TODO: Throw error when no video is loaded
	if (isColor)
	{
		videoReader->acquire(colorImage);  // get frame one by one
		display->init(colorImage, 0, 0, "Input images");
	}
	else
	{
		videoReader->acquire(grayImage);
		display->init(grayImage, 0, 0, "Input images");
	}
	
	frameIndex = videoReader->getFrameIndex();
}

void Vision::InitializeVideo(const std::string videoname, double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth)
{
	videoReader->setFileName(videoname);
	int videoWidth;
	int videoHeight;
	if (isColor)  // color images
	{
		videoReader->open(colorImage);
		videoWidth = colorImage.getWidth();
		videoHeight = colorImage.getHeight();

	}
	else  // gray images
	{
		videoReader->open(grayImage);
		videoWidth = grayImage.getWidth();
		videoHeight = grayImage.getHeight();
	}
	double videoFPS = videoReader->getFramerate();
	frameTime_ms = 1.0 / videoFPS * 1000.0;
	std::cout << "Video name     : " << videoname << std::endl;
	std::cout << "Video framerate: " << videoFPS << "fps" << std::endl;
	std::cout << "Video dimension: " << videoWidth << " " << videoHeight << std::endl;
	//TODO: Throw error when no video is loaded
	if (isColor)
	{

		videoReader->acquire(colorImage);  // get frame one by one
		if (selectroi == true) {
			SelectROI(roiTop, roiLeft, roiHeight, roiWidth);
		}
		display->init(colorImage, 0, 0, "Input images");
		//display->setWindowPosition(roiTop, roiLeft);
	}
	else
	{
		videoReader->acquire(grayImage);
		if (selectroi == true) {
			SelectROI(roiTop, roiLeft, roiHeight, roiWidth);
		}
		display->init(grayImage, 0, 0, "Input images");
	}
	
	frameIndex = videoReader->getFrameIndex();
}

void Vision::InitProcessedImageDisplay()
{
	const int xOffsetFromTopLeft = 1280;
	if (isColor)
	{
		vpImageConvert::convert(colorImage, processedgrayImage);
	}
	else
	{
		vpImageConvert::convert(grayImage, processedgrayImage);
	}
	
	processedImageDisplay->init(processedgrayImage, xOffsetFromTopLeft, 0, "Processed image");
	
}

void Vision::InitProcessedImageDisplay(double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth)
{
	const int xOffsetFromTopLeft = 1280;
	if (isColor)
	{
		videoReader->acquire(colorImage);  // get frame one by one
		display->init(colorImage, 0, 0, "Input images");
		vpImageConvert::convert(colorImage, processedgrayImage);
	}
	else
	{
		vpImageConvert::convert(grayImage, processedgrayImage);
	}

	// crop the image to get ROI (mannually)
	if (isSelectROI == true)
	{
		SelectROI(processedgrayImage, roiTop, roiLeft, roiHeight, roiWidth);
	}
	processedImageDisplay->init(processedgrayImage, xOffsetFromTopLeft, 0, "Processed image");
}

void Vision::InitTrackerImageDisplay()
{
	const int xOffsetFromTopLeft = 640;
	if (isColor)
	{
		vpImageConvert::convert(colorImage, trackerGrayImage);
	}
	else
	{
		vpImageConvert::convert(grayImage, trackerGrayImage);
	}
	trackerDisplay->init(trackerGrayImage, xOffsetFromTopLeft, 0, "Tracker image");
}

void Vision::InitTrackerImageDisplay(double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth)
{
	const int xOffsetFromTopLeft = 640;
	if (isColor)
	{
		vpImageConvert::convert(colorImage, trackerGrayImage);
	}
	else
	{
		vpImageConvert::convert(grayImage, trackerGrayImage);
	}
	
	// crop the image to get ROI (mannually)
	if (isSelectROI == true)
	{
		SelectROI(trackerGrayImage, roiTop, roiLeft, roiHeight, roiWidth);
	}
	trackerDisplay->init(trackerGrayImage, xOffsetFromTopLeft, 0, "Tracker image");
}



void Vision::AcquireImage()
{	
	if (isColor)
		videoReader->acquire(colorImage);
	else
		videoReader->acquire(grayImage);

	frameIndex = videoReader->getFrameIndex();
}

bool Vision::isVideoEnd()
{
	return videoReader->end();
}

double Vision::getFrameTime_ms()
{
	return frameTime_ms;
}

long Vision::getFrameIndex()
{
	return frameIndex;
}



// display the loaded raw images 
void Vision::DisplayImage()
{
	if (isColor)
	{
		display->display(colorImage);
	}
	else
	{
		display->display(grayImage);
	}
}

// display the processed images
void Vision::DisplayProcessedImage()
{
	processedImageDisplay->display(processedgrayImage);
}

// display the tracker images
void Vision::DisplayTrackerImage()
{
	trackerDisplay->display(trackerGrayImage);
}



// flush raw input images
void Vision::Flush()
{
	if (isColor)
	{
		display->flush(colorImage);
	}
	else
	{
		display->flush(grayImage);
	}
}

// flush processedgrayImage
void Vision::FlushProcessedImageDisplay()
{
	processedImageDisplay->flush(processedgrayImage);
}

// flush trackergrayImage
void Vision::FlushTrackerImageDisplay()
{
	trackerDisplay->flush(trackerGrayImage);
}



// convert raw input images into gray processed image
void Vision::ConvertToProcessed()
{
	if (isColor)
	{
		vpImageConvert::convert(colorImage, processedgrayImage);
	}
	else
	{
		vpImageConvert::convert(grayImage, processedgrayImage);
	}
}

// copy the pre-processed processedgrayImage to tracker 
void Vision::CopyProcessedToTracker()
{
	vpImageConvert::convert(processedgrayImage, trackerGrayImage);
}


// select ROI of gray image
void Vision::SelectROI(vpImage<unsigned char>& source, double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth)
{
	vpImage<unsigned char> gray;
	vpImageConvert::convert(source, gray);
	vpImageTools::crop(gray, roiTop, roiLeft, roiHeight, roiWidth, source, 1, 1);
}

// crop the raw input image (colorImage or grayImage)
void Vision::SelectROI(double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth)
{
	if (isColor)  // color images
	{

		std::cout << colorImage.getHeight() << std::endl;
		std::cout << colorImage.getWidth() << std::endl;
		colorTestImage = colorImage;
		vpImageTools::crop(colorTestImage, roiTop, roiLeft, roiHeight, roiWidth, colorImage);
		std::cout << 111 << std::endl;
	}

	else  // gray images
	{
		grayTestImage = grayImage;
		vpImageTools::crop(grayTestImage, roiTop, roiLeft, roiHeight, roiWidth, grayImage);
		//vpImageTools::crop(grayImage, 330, 360, 290, 290, grayImage, 1, 1);

		std::cout << 112 << std::endl;
	}



}

// crop the gray processed image
void Vision::Crop_Processed(double roiTop, double roiLeft, unsigned int roiHeight, unsigned int roiWidth)
{
	SelectROI(processedgrayImage, roiTop, roiLeft, roiHeight, roiWidth);
}



void Vision::TakeAScreenShot(const std::string videoname, std::string additionalInfo)
{
	size_t lastindex = videoname.find_last_of("\\");
	std::string rawname = videoname.substr(lastindex + 1);
	lastindex = rawname.find_last_of(".");
	rawname = rawname.substr(0, lastindex);
	rawname = rawname + "_";
	std::stringstream s1, s2;
	s1 << rawname << "processed_" << additionalInfo << ".jpeg";
	s2 << rawname << "raw_" << additionalInfo << ".jpeg";


	vpImageIo::writeJPEG(processedgrayImage, s1.str());
	if (isColor)
	{
		vpImageIo::writeJPEG(colorImage, s2.str());
	}
	else
	{
		vpImageIo::writeJPEG(grayImage, s2.str());
	}
		
}

void Vision::AutoScreenShot(std::string file) {
	int frameNumber =getFrameIndex();
	const int totalframes = 3;
	int frames[totalframes] = { 15,390, 400 };
	
	for (int frameId = 0; frameId < totalframes; frameId++)
	{
		if (frameNumber == frames[frameId]) 
			TakeAScreenShot(file, std::to_string(getFrameIndex()));
	}
}

void Vision::TakeAScreenShotYolo(const std::string videoname, std::string additionalInfo)
{
	size_t lastindex = videoname.find_last_of("\\");
	std::string rawname = videoname.substr(lastindex + 1);
	lastindex = rawname.find_last_of(".");
	rawname = rawname.substr(0, lastindex);
	rawname = rawname + "_";
	std::stringstream s1;
	s1 << rawname << "yolo_" << additionalInfo << ".jpeg";


	if (isColor)
	{
		vpImageIo::writeJPEG(colorImage, s1.str());
	}
	else
	{
		vpImageIo::writeJPEG(grayImage, s1.str());
	}

}



 
// stretchContrast on processedgrayImage
void Vision::stretchContrast()
{
	vp::stretchContrast(processedgrayImage, processedgrayImage);
	vp::unsharpMask(processedgrayImage, (float)0.5, 0.8);
}

// contrast on processedgrayImage
void Vision::Contrast(double alpha, double beta)
{
	vp::adjust(processedgrayImage, processedgrayImage, alpha, beta);
}

// VISP autoThreshold on processedgrayImage
void Vision::autoThreshold() 
{
	//vp::autoThreshold(processedgrayImage, vp::AUTO_THRESHOLD_HUANG);
	//vp::autoThreshold(processedgrayImage, vp::AUTO_THRESHOLD_INTERMODES);
	//vp::autoThreshold(processedgrayImage, vp::AUTO_THRESHOLD_ISODATA);
	//vp::autoThreshold(processedgrayImage, vp::AUTO_THRESHOLD_MEAN);
	vp::autoThreshold(processedgrayImage, vp::AUTO_THRESHOLD_OTSU, 0, 255);
	//vp::autoThreshold(processedgrayImage, vp::AUTO_THRESHOLD_TRIANGLE);
}

// OpenCV adaptiveThreshold on processedgrayImage
void Vision::adaptiveTresholding() 
{
	//cv::adaptiveThreshold(grayImage, processedgrayImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, \
		cv::THRESH_BINARY, 11, 2);

	cv::Mat gray;

	//convert(const vpImage< unsigned char > &src, cv::Mat & dest, bool copyData = true)
	vpImageConvert::convert(processedgrayImage, gray);

	cv::adaptiveThreshold(gray, gray, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 151, -2);

	//convert(const cv::Mat & src, vpImage< unsigned char > &dest, bool flip = false, unsigned int nThreads = 0)
	vpImageConvert::convert(gray, processedgrayImage);
}

// Guassian blurring
void Vision::GaussiaBlurring() 
{

	/*
	vpImageConvert::convert(processedgrayImage, grayImageOpenCV);

	cv::Mat Color;
	cvtColor(grayImageOpenCV, Color, cv::COLOR_GRAY2RGB);

	//cv::Mat Color; cvtColor(grayImageOpenCV, Color, cv::COLOR_GRAY2RGB);  // recover to color image

	cv::Mat norm, mediandst, gray, edges;

	//Histogram (strengthen the contrast)
	normalize(Color, norm, 255, 0, cv::NORM_MINMAX, CV_8U);

	//Median Filter
	medianBlur(norm, mediandst, 3);

	//conver to gray image
	cvtColor(mediandst, gray, cv::COLOR_BGR2GRAY);

	GaussianBlur(gray, gray, cv::Size(3, 3), 0.5, 0.5);

	vpImageConvert::convert(grayImageOpenCV, processedgrayImage);
	*/

	vpImage<double> processedgrayImageDouble;

	//vpImageConvert::convert(const vpImage< unsigned char > &src, vpImage< double > &dest);
	vpImageConvert::convert(processedgrayImage, processedgrayImageDouble);

	vpImageFilter::gaussianBlur(processedgrayImageDouble, processedgrayImageDouble, 11, 3);

	//vpImageConvert::convert(const vpImage< double > &src, vpImage< unsigned char > &dest);
	vpImageConvert::convert(processedgrayImageDouble, processedgrayImage);
}

// Guassian filter
void Vision::GaussianFiltering() 
{
	vpGaussianFilter gaussianFilter(processedgrayImage.getWidth(), processedgrayImage.getHeight(), 0.1);

	gaussianFilter.apply(processedgrayImage, processedgrayImage);

	//vpGaussianFilter(processedgrayImage.getWidth(), processedgrayImage.getHeight(), 0.5);
}


void Vision::vpHistogram()
{
	//if (isColor)
		//return;
	//else
	vpHistogram::vpHistogram(processedgrayImage);
}

void Vision::equalizeHistogram() {
	//if (isColor)
		//return;
	//else
	vp::equalizeHistogram(processedgrayImage);
}

void Vision::sharpen(float sigma, double weight) {
	//if (isColor)
		//vp::unsharpMask(processedcolorImage, sigma, weight);
	//else
	vp::unsharpMask(processedgrayImage, sigma, weight);
}

void Vision::gammaCorrection(double gamma) {
	//if (isColor)
		//vp::gammaCorrection(processedcolorImage, gamma);
	//else
	vp::gammaCorrection(processedgrayImage, gamma);
}



// VISP canny edge detection
void Vision::DetectEdges()
{
	vpImageFilter::canny(processedgrayImage, processedgrayImage, 7, 28, 3);
	vpDisplay::display(processedgrayImage);
}

// OpenCV canny edge detection
cv::Mat Vision::DetectEdgesOpenCV(double GaussianSigmaX, double GaussianSigmaY, double CannyThreshold1, double CannyThreshold2)
{
	vpImageConvert::convert(processedgrayImage, grayImageOpenCV);

	cv::Mat Color;
	cvtColor(grayImageOpenCV, Color, cv::COLOR_GRAY2RGB);  // recover to color image

	cv::Mat norm, mediandst, gray, edges;

	//Histogram (strengthen the contrast) 
	normalize(Color, norm, 255, 0, cv::NORM_MINMAX, CV_8U);

	//Median Filter
	medianBlur(norm, mediandst, 3);

	//conver to gray image
	cvtColor(mediandst, gray, cv::COLOR_BGR2GRAY);

	//Gaussian Filter
	GaussianBlur(gray, gray, cv::Size(3, 3), GaussianSigmaX, GaussianSigmaY);

	/* Canny detection */
	Canny(gray, edges, CannyThreshold1, CannyThreshold2);

	// Close calculation
	cv::Mat structureElement;
	structureElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
	cv::morphologyEx(edges, edges, cv::MORPH_CLOSE, structureElement, cv::Point(-1, -1), 1);
	// threshod
	cv::adaptiveThreshold(edges, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, -2);

	return edges;
}



////////////
// single tracker 
////////////

// VISP blob tracker
/*
bool Vision::blobtracking(bool init_done) 
{
	try 
	{
		if (!init_done) 
		{
			blob.setGraphics(true);
			blob.setGraphicsThickness(2);

			vpDisplay::display(trackerGrayImage);
			vpDisplay::displayText(trackerGrayImage, vpImagePoint(10, 10), "click in the blob to initialize the tracker", vpColor::red);
			if (vpDisplay::getClick(trackerGrayImage, germ, false)) 
			{
				blob.initTracking(trackerGrayImage, germ);
				init_done = true;
			}
		}
		else 
		{
			blob.track(trackerGrayImage);
			blob.display(trackerGrayImage, vpColor::red);
		}
	}
	catch (const vpException& e) 
	{
		std::cout << "Catch an exception: " << e << std::endl;
	}
	return init_done;
}
*/


/* VISP ellipse tracker */
// Ellipse tracking
bool Vision::ellipseTracking(bool ellipse_init_done)
{
	try 
	{
		if (!ellipse_init_done)
		{
			//auto disp_lane{ 0 };
			//trackerDisplay->displayText(trackerGrayImage, 15 * ++disp_lane, 15, "Left click to add five points", vpColor::red);
			//trackerDisplay->flush(trackerGrayImage);
			me.setRange(25);
			me.setThreshold(15000);
			me.setSampleStep(10);
			//std::cout << "you should select five points on ellipse" << endl;
			ellipse.setMe(&me);
			cout << me.getPointsToTrack() << endl;
			ellipse.setDisplay(vpMeSite::RANGE_RESULT);
			//ellipse.initTracking(trackerGrayImage);

			
			vpImagePoint initialpoints[initial_point_number];
			for (int i = 0; i < initial_point_number; i++)
			{
				initialpoints[i].set_ij(RectPoints[i].y, RectPoints[i].x);
			}
			ellipse.initTracking(trackerGrayImage, initial_point_number, initialpoints);
			
			ellipse_init_done = true;
			
		}
		else 
		{
			ellipse.track(trackerGrayImage);
			ellipse_paras.resize(ellipse_times+1);
			ellipse_paras[ellipse_times].resize(5);
			ellipse_paras[ellipse_times][0] = ellipse.getA();
			ellipse_paras[ellipse_times][1] = ellipse.getB();
			ellipse_paras[ellipse_times][2] = ellipse.getE();
			ellipse_paras[ellipse_times][3] = ellipse.getCenter().get_i();
			ellipse_paras[ellipse_times][4] = ellipse.getCenter().get_j();
			ellipse_times++;
			
			
			ellipse.display(trackerGrayImage, vpColor::red);
		}
	}
	catch (const vpException& e)
	{
		std::cout << "Catch an exception: " << e << std::endl;
	}
	return ellipse_init_done;
}

// save single ellipse tracker results
void Vision::SaveEllipseParas(std::string file)
{
	std::size_t found = file.find_last_of("/\\");
	string filename = file.substr(found + 1);
	std::cout << filename << std::endl;
	SaveMatlabMat(ellipse_paras, filename + ".mat");
}


/* OpenCV ellipse detector */
bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) 
{
	double i = fabs(contourArea(cv::Mat(contour1)));
	double j = fabs(contourArea(cv::Mat(contour2)));
	return (i < j);
}

auto Vision::FindContoursOfSingleDroplet_AreaBased(cv::Mat edge)
{
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	SingleDropletContour singleDropletContour;
	
	cv::findContours(edge, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	//std::cout << "Counter number: " << contours.size() << std::endl;
	std::sort(contours.begin(), contours.end(), compareContourAreas);


	//draw contour
	//drawContours(grayImageOpenCV, contours, contours.size() - 1, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
	//drawContours(grayImageOpenCV, contours, -1, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
	//vpImageConvert::convert(grayImageOpenCV, processedgrayImage);
	//vpDisplay::display(processedgrayImage);

	singleDropletContour.innerEdge.clear();
	if (contours.size() > 1)
	{
		singleDropletContour.innerEdge = contours[contours.size() - 2];
		singleDropletContour.outerEdge = contours[contours.size() - 1];
	}
	else
	{
		singleDropletContour.innerEdge = contours[contours.size() - 1];
		singleDropletContour.outerEdge = contours[contours.size() - 1];
	}
	
	contours[contours.size() - 1];
	//std::cout << singleDropletContour.innerEdge.size() << endl;
	//std::cout << singleDropletContour.outerEdge.size() << endl;
	return singleDropletContour;
}

void Vision::DrawEllipseOnImage(cv::RotatedRect RotRect)
{
	
	cv::Point2f pts[4];
	RotRect.points(pts);

	cv::ellipse(grayImageOpenCV, RotRect, cv::Scalar(0, 0, 255), 1, 8); //22 7 2021 : added cv:: to remove conflict with visp vision ellipse
	cv::line(grayImageOpenCV, (pts[0] + pts[3]) / 2, (pts[1] + pts[2]) / 2,	cv::Scalar(0, 0, 255), 1, cv::LINE_AA );
	cv::line(grayImageOpenCV, (pts[0] + pts[1]) / 2, (pts[2] + pts[3]) / 2, cv::Scalar(0, 0, 255), 1, cv::LINE_AA );
	vpImageConvert::convert(grayImageOpenCV, processedgrayImage);

	vpDisplay::display(processedgrayImage);
}

/*
Fits an ellipse for videos with single droplet with elliptical shape, using opencv
returns minorAxisLength, majorAxisLength, majorAxisAngle
*/
std::tuple<double, double, double> Vision::FitEllipse(cv::Mat edge, bool drawEllipse, bool printEllipseInfoOnImage )
{
	cv::Point2f pts[4];
	double minorAxisLength = 0.0;
	double majorAxisLength = 0.0;
	double majorAxisAngle = 0.0;

	
	auto singleDropletContour = FindContoursOfSingleDroplet_AreaBased(edge); 
	
	cv::RotatedRect RotRect = cv::fitEllipse(cv::Mat(singleDropletContour.outerEdge)); 
		
	RotRect.points(pts);
	if (drawEllipse)
	{
		DrawEllipseOnImage(RotRect);
	}

	int counter_points_number = singleDropletContour.outerEdge.size();
	int step = counter_points_number / initial_point_number;
	//cout << counter_points_number << " " << step << endl;

	for (int i = 0; i < initial_point_number; i++)
	{
		//if (i < 4)
			//RectPoints[i] = pts[i];
		//else
		RectPoints[i] = singleDropletContour.outerEdge[i * step];
	}

	
	//identify the axes and angle
	if (cv::norm(pts[0] - pts[1]) < cv::norm(pts[1] - pts[2]))
	{
		minorAxisLength = cv::norm(pts[0] - pts[1]);
		majorAxisLength = cv::norm(pts[1] - pts[2]);
		if (pts[2].y < pts[1].y)
			majorAxisAngle = atan2(-(pts[2].y - pts[1].y), pts[2].x - pts[1].x);
		else
			majorAxisAngle = atan2(-(pts[1].y - pts[2].y), pts[1].x - pts[2].x);
	}
	else
	{
		minorAxisLength = cv::norm(pts[1] - pts[2]);
		majorAxisLength = cv::norm(pts[0] - pts[1]);
		if (pts[1].y < pts[0].y)
			majorAxisAngle = atan2(-(pts[1].y - pts[0].y), pts[1].x - pts[0].x);
		else
			majorAxisAngle = atan2(-(pts[0].y - pts[1].y), pts[0].x - pts[1].x);
	}
	//convert to degrees
	majorAxisAngle = majorAxisAngle / deg2rad;
	  
	return std::tuple<double, double, double>(minorAxisLength, majorAxisLength, majorAxisAngle);
}



///////////////
// Multi-droplet tracking: the process result is stored in processedgrayImage
///////////////

// initialize the OpenCV Blob detector
void Vision::InitOpenCVBlobDetector(cv::SimpleBlobDetector::Params params)
{
	// Set up detector with params
	detector = cv::SimpleBlobDetector::create(params);
}

std::vector<vpImagePoint> Vision::OpenCVDetectBlobs()
{
	/*
	before calling this function, you must perform
	ConvertToProcessed() to obtain the grayImageOpenCV.
	*/

	vpImageConvert::convert(processedgrayImage, grayImageOpenCV);
	
	std::vector<cv::KeyPoint> keypoints;
	std::vector<vpImagePoint> detectedPoints;
	detector->detect(grayImageOpenCV, keypoints);
	//std::cout << keypoints.size() << endl;
	vpImageConvert::convert(grayImageOpenCV, processedgrayImage);
	vpConvert::convertFromOpenCV(keypoints, detectedPoints);

	return detectedPoints;
}

std::vector<vpImagePoint> Vision::GetUserClickedPoints()
{
	// print some information on raw input image
	auto disp_lane{ 0 };
	if (isColor)
	{
		display->displayText(colorImage, 15 * ++disp_lane, 15, "Left click to add a point", vpColor::black);
		display->displayText(colorImage, 15 * ++disp_lane, 15, "Middle click to continue (run Munkres)", vpColor::black);
		display->displayText(colorImage, 15 * ++disp_lane, 15, "Right click to quit", vpColor::black);
	}
	else
	{
		display->displayText(grayImage, 15 * ++disp_lane, 15, "Left click to add a point", vpColor::black);
		display->displayText(grayImage, 15 * ++disp_lane, 15, "Middle click to continue (run Munkres)", vpColor::black);
		display->displayText(grayImage, 15 * ++disp_lane, 15, "Right click to quit", vpColor::black);
	}
	Flush();
	

	// Ask user to clic on point
	std::vector<vpImagePoint> user_ips{};
	vpMouseButton::vpMouseButtonType button{};
	while (button != vpMouseButton::button2) 
	{
		vpImagePoint ip{};

		if (isColor)
			vpDisplay::getClick(colorImage, ip, button, true);
		else
			vpDisplay::getClick(grayImage, ip, button, true);

		
		if (button == vpMouseButton::button1)
		{
			user_ips.push_back(ip);
		}
		else if (button == vpMouseButton::button3)
		{
			break;
		}
		for each (auto point in user_ips)
		{
			if (isColor)
				vpDisplay::displayCross(colorImage, point.get_i(), point.get_j(), 25, vpColor::blue, 1);
			else
				vpDisplay::displayCross(grayImage, point.get_i(), point.get_j(), 25, vpColor::blue, 1);
		}
		Flush();
	}
	return user_ips;
}

vpColVector Vision::convert_vpImagePointTo_vpColVector(vpImagePoint point)
{
	vpColVector vec = vpColVector(2);
	vec[0] = point.get_u();
	vec[1] = point.get_v();
	return vec;
}

// display the detected droplet with circle
void Vision::DisplayCircle(vpImagePoint ip, vpColor color)
{
	vpDisplay::displayCircle(processedgrayImage, ip, 5, color, true, 1);
}

// display the detected droplet with cross
void Vision::DisplayCross(vpImagePoint ip, vpColor color)
{
	vpDisplay::displayCross(processedgrayImage, ip.get_i(), ip.get_j(), 25, color, 1);
}



///////////////
// Deep learning detector: yolo
///////////////

bool Vision::Yolo(bool yolo_init, std::string ConfigPath,std::string ParameterPath)
{
	if (yolo_init == false)
	{
		std::ifstream file("obj.names");
		std::string line;
		while (std::getline(file, line)) 
		{
			classes.push_back(line);
		}

		Net net = readNetFromDarknet(ConfigPath, ParameterPath);
		model = DetectionModel(net);

		model.setInputParams(1 / 255.0, Size(320, 320), Scalar(), true);
		
		return true;

	}

	if (isColor)
	{
		vpImageConvert::convert(Vision::colorImage, Vision::colorImageOpenCV_yolo);
		model.detect(colorImageOpenCV_yolo, classIds, scores, boxes, 0.6, 0.4);
		for (int i = 0; i < classIds.size(); i++)
		{
			rectangle(colorImageOpenCV_yolo, boxes[i], Scalar(0, 255, 0), 2);
		}
		vpImageConvert::convert(Vision::colorImageOpenCV_yolo, Vision::colorImage);
		vpDisplay::display(colorImage);
	}
	else
	{
		vpImageConvert::convert(Vision::grayImage, Vision::grayImageOpenCV_yolo);
		cvtColor(Vision::grayImageOpenCV_yolo, Vision::colorImageOpenCV_yolo, cv::COLOR_GRAY2RGB);
		model.detect(colorImageOpenCV_yolo, classIds, scores, boxes, 0.6, 0.4);
		for (int i = 0; i < classIds.size(); i++)
		{
			rectangle(colorImageOpenCV_yolo, boxes[i], Scalar(0, 255, 0), 2);
		}
		cvtColor(colorImageOpenCV_yolo, grayImageOpenCV_yolo, cv::COLOR_BGR2GRAY);
		vpImageConvert::convert(Vision::grayImageOpenCV_yolo, Vision::grayImage);
		vpDisplay::display(grayImage);
	}

}


std::vector<cv::Rect> Vision::MultiTrackingYolo(std::string ConfigPath, std::string ParameterPath, bool first_run)
{

	if (first_run)
	{
		std::ifstream file("obj.names");
		std::string line;
		while (std::getline(file, line))
		{
			classes.push_back(line);
		}

		Net net = readNetFromDarknet(ConfigPath, ParameterPath);
		model = DetectionModel(net);

		model.setInputParams(1 / 255.0, Size(320, 320), Scalar(), true);

		return {};
	}


	if (isColor)
	{
		vpImageConvert::convert(Vision::colorImage, Vision::colorImageOpenCV_yolo);
		model.detect(colorImageOpenCV_yolo, classIds, scores, boxes, 0.6, 0.4);
		vpImageConvert::convert(Vision::colorImageOpenCV_yolo, Vision::colorImage);
	}
	else
	{
		vpImageConvert::convert(Vision::grayImage, Vision::grayImageOpenCV_yolo);
		cvtColor(Vision::grayImageOpenCV_yolo, Vision::colorImageOpenCV_yolo, cv::COLOR_GRAY2RGB);
		model.detect(colorImageOpenCV_yolo, classIds, scores, boxes, 0.6, 0.4);
		cvtColor(colorImageOpenCV_yolo, grayImageOpenCV_yolo, cv::COLOR_BGR2GRAY);
		vpImageConvert::convert(Vision::grayImageOpenCV_yolo, Vision::grayImage);
	}

	return boxes;
}





