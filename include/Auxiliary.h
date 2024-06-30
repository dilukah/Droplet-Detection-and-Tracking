#pragma once

#include <iostream>
#include <vector>


class EdgeDetectionClass 
{

public:
	EdgeDetectionClass(double a, double b, double c, double d, double e, double f, unsigned int g, unsigned int h);
	double get_GaussianSigmaX();
	double get_GaussianSigmaY();
	double get_CannyThreshold1();
	double get_CannyThreshold2();
	double get_roiTop();
	double get_roiLeft();
	unsigned int get_roiHeight();
	unsigned int get_roiWidth();
	std::vector<double> get_all_parameters();

private:
	double _GaussianSigmaX;
	double _GaussianSigmaY;
	double _CannyThreshold1;
	double _CannyThreshold2;
	double _roiTop;
	double _roiLeft;
	unsigned int _roiHeight;
	unsigned int _roiWidth;
};

class MultiTrackingClass 
{

public:
	MultiTrackingClass(double a, double b, double c, double d, double e, double f, bool g,
		bool h, bool i, bool j, double k, double l, unsigned int m, unsigned int n);

	double get_minThreshold();
	double get_maxThreshold();
	double get_minArea();
	double get_minCircularity();
	double get_minConvexity();
	double get_minInertiaRatio();
	bool get_selectroi();
	bool get_treshold();
	bool get_YOLOinit();
	bool get_YOLOdetect();
	double get_roiTop();
	double get_roiLeft();
	unsigned int get_roiHeight();
	unsigned int get_roiWidth();
	std::vector<double> get_all_parameters();

private:
	double _minThreshold;
	double _maxThreshold;
	double _minArea;
	double _minCircularity;
	double _minConvexity;
	double _minInertiaRatio;
	bool _selectroi; 
	bool _treshold;
	bool _YOLOinit;
	bool _YOLOdetect;
	double _roiTop;
	double _roiLeft;
	unsigned int _roiHeight;
	unsigned int _roiWidth;
};

class YoloDetectionClass 
{

public:
	YoloDetectionClass(std::string a, std::string b);

	std::string get_yoloConfigPath();
	std::string get_modelParametersPath();
	std::vector<double> get_all_parameters();

private:
	std::string _yoloConfigPath;
	std::string _modelParametersPath;
};

EdgeDetectionClass* LoadParametersEdgeDetection();

MultiTrackingClass* LoadParametersMultiTracking();

YoloDetectionClass* LoadParametersYoloDetection();