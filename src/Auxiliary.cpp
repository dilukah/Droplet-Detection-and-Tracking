#include "Auxiliary.h"
#include "yaml-cpp/yaml.h"


EdgeDetectionClass::EdgeDetectionClass(double a, double b, double c, double d, double e, double f, unsigned int g, unsigned int h) :
	_GaussianSigmaX(a), _GaussianSigmaY(b), _CannyThreshold1(c), _CannyThreshold2(d),
	_roiTop(e), _roiLeft(f), _roiHeight(g), _roiWidth(h) { }

double EdgeDetectionClass::get_GaussianSigmaX()
{
	return _GaussianSigmaX;
}

double EdgeDetectionClass::get_GaussianSigmaY()
{
	return _GaussianSigmaY;
}

double EdgeDetectionClass::get_CannyThreshold1() 
{
	return _CannyThreshold1;
}

double EdgeDetectionClass::get_CannyThreshold2() 
{
	return _CannyThreshold2;
}

double EdgeDetectionClass::get_roiTop()
{
	return _roiTop;
}

double EdgeDetectionClass::get_roiLeft()
{
	return _roiLeft;
}

unsigned int EdgeDetectionClass::get_roiHeight() 
{
	return _roiHeight;
}

unsigned int EdgeDetectionClass::get_roiWidth() 
{
	return _roiWidth;
}

std::vector<double> EdgeDetectionClass::get_all_parameters()
{
	return { _GaussianSigmaX, _GaussianSigmaY, _CannyThreshold1, _CannyThreshold2,
		_roiTop, _roiLeft, (double)_roiHeight, (double)_roiWidth };
}



MultiTrackingClass::MultiTrackingClass(double a, double b, double c, double d, double e, double f, bool g,
	bool h, bool i, bool j, double k, double l, unsigned int m, unsigned int n) : 
	_minThreshold(a), _maxThreshold(b), _minArea(c), _minCircularity(d), _minConvexity(e),
	_minInertiaRatio(f), _selectroi(g), _treshold(h), _YOLOinit(i), _YOLOdetect(j), _roiTop(k), _roiLeft(l), _roiHeight(m), _roiWidth(n) { }

double MultiTrackingClass::get_minThreshold() 
{
	return _minThreshold;
}

double MultiTrackingClass::get_maxThreshold() 
{
	return _maxThreshold;
}

double MultiTrackingClass::get_minArea() 
{
	return _minArea;
}

double MultiTrackingClass::get_minCircularity() 
{
	return _minCircularity;
}

double MultiTrackingClass::get_minConvexity() 
{
	return _minConvexity;
}

double MultiTrackingClass::get_minInertiaRatio() 
{
	return _minInertiaRatio;
}


bool MultiTrackingClass::get_selectroi()
{
	return _selectroi;
}

bool MultiTrackingClass::get_treshold()
{
	return _treshold;
}

bool MultiTrackingClass::get_YOLOinit()
{
	return _YOLOinit;
}

bool MultiTrackingClass::get_YOLOdetect()
{
	return _YOLOdetect;
}

double MultiTrackingClass::get_roiTop()
{
	return _roiTop;
}

double MultiTrackingClass::get_roiLeft()
{
	return _roiLeft;
}

unsigned int MultiTrackingClass::get_roiHeight() 
{
	return _roiHeight;
}
unsigned int MultiTrackingClass::get_roiWidth() 
{
	return _roiWidth;
}

std::vector<double> MultiTrackingClass::get_all_parameters() 
{
	return { _minThreshold, _maxThreshold, _minArea, _minCircularity, _minConvexity,
		_minInertiaRatio, _roiTop, _roiLeft, (double)_roiHeight, (double)_roiWidth };
}



YoloDetectionClass::YoloDetectionClass(std::string a, std::string b) : _yoloConfigPath(a), _modelParametersPath(b) { }

std::string YoloDetectionClass::get_yoloConfigPath() 
{
	return _yoloConfigPath;
}

std::string YoloDetectionClass::get_modelParametersPath() 
{
	return _modelParametersPath;
}

std::vector<double> YoloDetectionClass::get_all_parameters() 
{
	return { };
}

EdgeDetectionClass* LoadParametersEdgeDetection()
{
	std::string filename = "config/parameters.yaml";
	std::string method = "EdgeDetection";

	YAML::Node config = YAML::LoadFile(filename);
	std::string filepath = config[method]["filepath"].as<std::string>();

	//if (get_videofilepath) return filepath;

	double GaussianSigmaX;
	double GaussianSigmaY;
	double CannyThreshold1;
	double CannyThreshold2;

	double roiTop;
	double roiLeft;
	unsigned int roiHeight;
	unsigned int roiWidth;

	GaussianSigmaX = config[method]["GaussianSigmaX"].as<double>();
	GaussianSigmaY = config[method]["GaussianSigmaY"].as<double>();

	CannyThreshold1 = config[method]["CannyThreshold1"].as<double>();
	CannyThreshold2 = config[method]["CannyThreshold2"].as<double>();

	roiTop = config[method]["roiTop"].as<double>();
	roiLeft = config[method]["roiLeft"].as<double>();
	roiHeight = config[method]["roiHeight"].as<unsigned int>();
	roiWidth = config[method]["roiWidth"].as<unsigned int>();


	std::cout << "The following parameters were loaded from " << "'" << filename << "'" << ":" << std::endl;
	std::cout << "Filepath for the videos: " << filepath << std::endl;
	std::cout << "Method of detection/tracking: " << method << std::endl;
	std::cout << "GaussianSigmaX: " << GaussianSigmaX << std::endl;
	std::cout << "GaussianSigmaY: " << GaussianSigmaY << std::endl;
	std::cout << "CannyThreshold1: " << CannyThreshold1 << std::endl;
	std::cout << "CannyThreshold2: " << CannyThreshold2 << std::endl;
	std::cout << std::endl;

	EdgeDetectionClass* obj_ptr = new EdgeDetectionClass(GaussianSigmaX, GaussianSigmaY, CannyThreshold1,
		CannyThreshold2, roiTop, roiLeft, roiHeight, roiWidth);

	return obj_ptr;
}

MultiTrackingClass* LoadParametersMultiTracking()
{
	std::string filename = "config/parameters.yaml";
	std::string method = "MultiDropletTracking";

	YAML::Node config = YAML::LoadFile(filename);
	std::string filepath = config[method]["filepath"].as<std::string>();

	double minThreshold;
	double maxThreshold;
	double minArea;
	double minCircularity;
	double minConvexity;
	double minInertiaRatio;
	bool selectroi;
	bool treshold;
	bool YOLOinit;
	bool YOLOdetect;

	double roiTop;
	double roiLeft;
	unsigned int roiHeight;
	unsigned int roiWidth;

	minThreshold = config[method]["minThreshold"].as<double>();
	maxThreshold = config[method]["maxThreshold"].as<double>();
	minArea = config[method]["minArea"].as<double>();
	minCircularity = config[method]["minCircularity"].as<double>();
	minConvexity = config[method]["minConvexity"].as<double>();
	minInertiaRatio = config[method]["minInertiaRatio"].as<double>();
	selectroi = (bool)config[method]["selectroi"].as<int>();
	treshold = (bool)config[method]["treshold"].as<int>();
	YOLOinit = (bool)config[method]["YOLOinit"].as<int>();
	YOLOdetect = (bool)config[method]["YOLOdetect"].as<int>();

	roiTop = config[method]["roiTop"].as<double>();
	roiLeft = config[method]["roiLeft"].as<double>();
	roiHeight = config[method]["roiHeight"].as<unsigned int>();
	roiWidth = config[method]["roiWidth"].as<unsigned int>();



	std::cout << "The following parameters were loaded from " << "'" << filename << "'" << ":" << std::endl;
	std::cout << "Filepath for the videos: " << filepath << std::endl;
	std::cout << "Method of detection/tracking: " << method << std::endl;
	std::cout << "minThreshold: " << minThreshold << std::endl;
	std::cout << "maxThreshold: " << maxThreshold << std::endl;
	std::cout << "minArea: " << minArea << std::endl;
	std::cout << "minCircularity: " << minCircularity << std::endl;
	std::cout << "minConvexity: " << minConvexity << std::endl;
	std::cout << "minInertiaRatio: " << minInertiaRatio << std::endl;
	std::cout << std::endl;

	MultiTrackingClass* obj_ptr = new MultiTrackingClass(minThreshold, maxThreshold, minArea,
		minCircularity, minConvexity, minInertiaRatio, selectroi, treshold, YOLOinit, YOLOdetect, roiTop, roiLeft, roiHeight, roiWidth);

	return obj_ptr;
}

YoloDetectionClass* LoadParametersYoloDetection()
{
	std::string filename = "config/parameters.yaml";
	std::string method = "YoloDetection";

	YAML::Node config = YAML::LoadFile(filename);
	std::string filepath = config[method]["filepath"].as<std::string>();

	std::string yoloConfigPath;
	std::string modelParametersPath;

	yoloConfigPath = config[method]["yoloConfigPath"].as<std::string>();
	modelParametersPath = config[method]["modelParametersPath"].as<std::string>();


	std::cout << "The following parameters were loaded from " << "'" << filename << "'" << ":" << std::endl;
	std::cout << "Filepath for the videos: " << filepath << std::endl;
	std::cout << "Filepath for the Yolo algorithm configuration: " << yoloConfigPath << std::endl;
	std::cout << "Filepath for the Yolo model parameters: " << modelParametersPath << std::endl;
	std::cout << std::endl;

	YoloDetectionClass* obj_ptr = new YoloDetectionClass(yoloConfigPath, modelParametersPath);

	return obj_ptr;
}


/*
// the function used to test the MatExport
void test_Export()
{
	std::vector<std::vector<double> > result1(5, std::vector<double>(3));
	std::vector<std::vector<double> > result2(5, std::vector<double>(3));

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result1[i][j] = i + j + 0.1;
			result2[i][j] = i * j + 0.5;
		}
	}

	int size_col = result1[0].size(); //获取列数
	int size_row = result1.size(); //获取行数
	std::cout << result1[0].size() << std::endl;
	std::cout << result1.size() << std::endl;

	for (int i = 0; i < size_row; i++) {
		for (int j = 0; j < size_col; j++)
			std::cout << result1[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < size_row; i++) {
		for (int j = 0; j < size_col; j++)
			std::cout << result2[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;


	std::string m = "test.mat";
	SaveMatlabMat(result1, result2, m);
}


void DisplayConfig()
{
	YAML::Node config = YAML::LoadFile("config.yaml");
	const std::string username = config["username"].as<std::string>();
	const std::string password = config["password"].as<std::string>();
	std::cout << "Username: " << username << std::endl;
	config["username"] = "Corey Hart";
	//changeusername in config.yaml after you run it
	std::ofstream fout("config.yaml");
	fout << config;
}


const int MAX = 10;

void imageSwap(int mat[][MAX], int n)
{
	// for diagonal which start from at
	// first row of matrix
	int row = 0;

	// traverse all top right diagonal
	for (int j = 0; j < n; j++) {

		// here we use stack for reversing
		// the element of diagonal
		stack<int> s;
		int i = row, k = j;
		while (i < n && k >= 0)
			s.push(mat[i++][k--]);

		// push all element back to matrix
		// in reverse order
		i = row, k = j;
		while (i < n && k >= 0) {
			mat[i++][k--] = s.top();
			s.pop();
		}
	}

	// do the same process for all the
	// diagonal which start from last
	// column
	int column = n - 1;
	for (int j = 1; j < n; j++) {

		// here we use stack for reversing
		// the elements of diagonal
		stack<int> s;
		int i = j, k = column;
		while (i < n && k >= 0)
			s.push(mat[i++][k--]);

		// push all element back to matrix
		// in reverse order
		i = j;
		k = column;
		while (i < n && k >= 0) {
			mat[i++][k--] = s.top();
			s.pop();
		}
	}
}*/
