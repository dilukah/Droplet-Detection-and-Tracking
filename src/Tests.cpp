#include <Tests.h>
#include "Auxiliary.h"
#include <Vision.h>

/*
* Uses opencv simpleblobdetector to detect blobs. Tracking using munkres algorithm
* TODO: Improve detection
*	Try thresholding and providing a binary image to blob detector to improve detection.
*	Try different initialization parameters for simple blob detector
*/

std::vector<vpImagePoint> Test_MultiDropletTracking(Vision& MyVision, MultiTrackingClass* parameters)
{
	double minThreshold = parameters->get_minThreshold();
	double maxThreshold = parameters->get_maxThreshold();
	double minArea = parameters->get_minArea();
	double minCircularity = parameters->get_minCircularity();
	double minConvexity = parameters->get_minConvexity();
	double minInertiaRatio = parameters->get_minInertiaRatio();

	double roiTop = parameters->get_roiTop();
	double roiLeft = parameters->get_roiLeft();
	unsigned int roiHeight = parameters->get_roiHeight();
	unsigned int roiWidth = parameters->get_roiWidth();

	std::vector<vpImagePoint> detectedPoints;
	static bool firstRun = false;
	
	// input -> gaussian filter -> treshold -> filter

	MyVision.AcquireImage();
	if (MyVision.selectroi) {
		MyVision.SelectROI(roiTop, roiLeft, roiHeight, roiWidth);
	}
	
	//TODO:Try different procesing techniques
	MyVision.ConvertToProcessed();
	MyVision.stretchContrast();
	
	//MyVision.vpHistogram();
	//MyVision.equalizeHistogram();
	MyVision.gammaCorrection(0.4);
	
	MyVision.GaussiaBlurring();
	MyVision.sharpen(0.6, 0.1);
	
	//MyVision.GaussianFiltering();

	if (MyVision.treshold) {
		MyVision.autoThreshold();
		//MyVision.adaptiveTresholding();
	}

	//MyVision.GaussiaBlurring();
	MyVision.GaussianFiltering();
	
	MyVision.DisplayImage();
	MyVision.DisplayProcessedImage();

	
	if (firstRun == false)
	{
		// Setup SimpleBlobDetector parameters.
		cv::SimpleBlobDetector::Params params;

		// Change thresholds
		params.minThreshold = minThreshold;
		params.maxThreshold = maxThreshold;

		// Filter by Area.
		params.filterByArea = true;
		params.minArea = minArea;
		
		// Filter by Circularity
		params.filterByCircularity = true;
		params.minCircularity = minCircularity;

		// Filter by Convexity
		params.filterByConvexity = true;
		params.minConvexity = minConvexity;

		// Filter by Inertia
		params.filterByInertia = true;
		params.minInertiaRatio = minInertiaRatio;

		MyVision.InitOpenCVBlobDetector(params); 

		detectedPoints = MyVision.GetUserClickedPoints();
		firstRun = true;
	}
	else
	{
		detectedPoints = MyVision.OpenCVDetectBlobs();
	}

	auto trackedPoints = MyVision.TrackMunkres(detectedPoints);

	MyVision.DisplayCross(detectedPoints.at(0), vpColor::red);
	for (int i = 0; i < detectedPoints.size(); i++)
	{
		MyVision.DisplayCross(detectedPoints[i], vpColor::red);
	}
	std::cout << "Number of Tracked Points: " << trackedPoints.size() << std::endl;
	for (int i = 0; i < trackedPoints.size(); i++)
	{
		MyVision.DisplayCross(trackedPoints[i], vpColor::blue);
	}

	return trackedPoints;

}


std::vector<vpImagePoint> Test_MultiDropletTracking(Vision& MyVision, MultiTrackingClass* parameters, YoloDetectionClass* other_parameters)
{

	double minThreshold = parameters->get_minThreshold();
	double maxThreshold = parameters->get_maxThreshold();
	double minArea = parameters->get_minArea();
	double minCircularity = parameters->get_minCircularity();
	double minConvexity = parameters->get_minConvexity();
	double minInertiaRatio = parameters->get_minInertiaRatio();

	bool YOLOdetect = parameters->get_YOLOdetect();

	double roiTop = parameters->get_roiTop();
	double roiLeft = parameters->get_roiLeft();
	unsigned int roiHeight = parameters->get_roiHeight();
	unsigned int roiWidth = parameters->get_roiWidth();

	std::string ConfigPath = other_parameters->get_yoloConfigPath();
	std::string ParameterPath = other_parameters->get_modelParametersPath();

	std::vector<vpImagePoint> detectedPoints;
	static bool firstRun = false;


	MyVision.AcquireImage();
	if (MyVision.selectroi) {
		MyVision.SelectROI(roiTop, roiLeft, roiHeight, roiWidth);
	}

	//TODO:Try different procesing techniques
	MyVision.ConvertToProcessed();
	MyVision.stretchContrast();

	MyVision.gammaCorrection(0.4);
	MyVision.GaussiaBlurring();
	MyVision.sharpen(0.6, 0.1);

	//MyVision.GaussianFiltering();

	if (MyVision.treshold) {
		MyVision.autoThreshold();
		//MyVision.adaptiveTresholding();
	}

	MyVision.GaussianFiltering();

	MyVision.DisplayImage();
	MyVision.DisplayProcessedImage();


	if (firstRun == false)
	{
		// Setup SimpleBlobDetector parameters.
		cv::SimpleBlobDetector::Params params;

		// Change thresholds
		params.minThreshold = minThreshold;
		params.maxThreshold = maxThreshold;

		// Filter by Area.
		params.filterByArea = true;
		params.minArea = minArea;


		// Filter by Circularity
		params.filterByCircularity = true;
		params.minCircularity = minCircularity;

		// Filter by Convexity
		params.filterByConvexity = true;
		params.minConvexity = minConvexity;

		// Filter by Inertia
		params.filterByInertia = true;
		params.minInertiaRatio = minInertiaRatio;

		MyVision.InitOpenCVBlobDetector(params);

		// Init yolo
		MyVision.MultiTrackingYolo(ConfigPath, ParameterPath, true);

		std::vector<cv::Rect> boxes = MyVision.MultiTrackingYolo(ConfigPath, ParameterPath);

		for (auto box : boxes) {
			vpImagePoint newpoint(box.y + box.height * 0.5, box.x + box.width * 0.5);
			detectedPoints.push_back(newpoint);
		}

		firstRun = true;
	}
	else
	{
		if (YOLOdetect) {
			std::vector<cv::Rect> boxes = MyVision.MultiTrackingYolo(ConfigPath, ParameterPath);

			for (auto box : boxes) {
				vpImagePoint newpoint(box.y + box.height * 0.5, box.x + box.width * 0.5);
				detectedPoints.push_back(newpoint);
			}
		}
		else {
			detectedPoints = MyVision.OpenCVDetectBlobs();
		}
	}

	auto trackedPoints = MyVision.TrackMunkres(detectedPoints);

	MyVision.DisplayCross(detectedPoints.at(0), vpColor::red);
	for (int i = 0; i < detectedPoints.size(); i++)
	{
		MyVision.DisplayCross(detectedPoints[i], vpColor::red);
	}
	std::cout << "Number of Tracked Points: " << trackedPoints.size() << std::endl;
	for (int i = 0; i < trackedPoints.size(); i++)
	{
		MyVision.DisplayCross(trackedPoints[i], vpColor::blue);
	}

	return trackedPoints;

}
