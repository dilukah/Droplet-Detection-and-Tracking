#include <iostream>
#include <cctype>
#include <fstream>  
#include <filesystem>  
#include "Vision.h"
#include "Utility.h"
#include "LoadFiles.h"
#include "yaml-cpp/yaml.h"
#include "Tests.h"
#include "MatlabExport.h"
#include <vector>
#include "Auxiliary.h"


namespace fs = std::filesystem;

std::array<int, 4> LoadKeyBinds(bool quick = false);
std::string LoadVideoFilepath(std::string method);
template<class T>
std::string CreateDirectory(std::string method, T* parameter_class, bool quick = false);
std::vector<std::string> LoadVideos(LoadFiles& loadfileobj);
int EdgeDetection(std::vector<double> parameters, std::array<int, 4> keybinds,
	std::vector<std::string> files, bool colorVideo = true, bool ROI_flag = true);
int YoloDetection(YoloDetectionClass* parameters, std::array<int, 4> keybinds,
	std::vector<std::string> files, bool colorVideo = true);
int MultiDropletTracking(MultiTrackingClass* parameters, std::array<int, 4> keybinds,
	std::vector<std::string> files);


int main(int argc, char* argv[])
{

	std::cout << "\n -- Program starting... -- \n" << std::endl;

	fs::current_path("../../"); // First get to directory "liquid-tracking/".

	bool quick = true; // For faster testing.


	std::cout << "Please select a number from 1, 2 and 3 corresponding to the method of detection/tracking you want to perform:" << std::endl;
	std::cout << "1: Conventional method to detect and track a single droplet" << std::endl;
	std::cout << "2: Conventional method to detect and track multiple droplets" << std::endl;
	std::cout << "3: Deep learning method to detect a single droplet or multiple droplets" << std::endl;

	int method_index;
	bool method_ok = 1;

type: {

	if (!method_ok) std::cout << "The valid options are 1, 2 and 3!" << std::endl;
	std::cout << "Method of detection/tracking: ";
	std::cin >> method_index; }

std::string option;
std::string method;

if (method_index == 1) {
	option = "--edge";
}
else if (method_index == 2) {
	option = "--multi";
}
else if (method_index == 3) {
	option = "--yolo";
}
else {
	method_ok = 0;
	goto type;
}


std::cout << "\n -- Loading parameters... --\n" << std::endl;

// Get the parameters and video file path for the corresponding method of droplet detection and tracking.
EdgeDetectionClass* parameters1;
MultiTrackingClass* parameters2;
YoloDetectionClass* parameters3;

if (option == "--edge")
{
	method = "EdgeDetection";
	parameters1 = LoadParametersEdgeDetection();
}
else if (option == "--multi")
{
	method = "MultiDropletTracking";
	parameters2 = LoadParametersMultiTracking();
}
else if (option == "--yolo") {
	method = "YoloDetection";
	parameters3 = LoadParametersYoloDetection();
}

std::string filepath = LoadVideoFilepath(method);


std::cout << " -- Configuring the save directory... --\n" << std::endl;

// The directory where the material will be stored.
std::string save_directory;

if (method == "EdgeDetection") {
	save_directory = CreateDirectory<EdgeDetectionClass>(method, parameters1, quick);
}
else if (method == "MultiDropletTracking") {
	save_directory = CreateDirectory<MultiTrackingClass>(method, parameters2, quick);
}
else if (method == "YoloDetection") {
	save_directory = CreateDirectory<YoloDetectionClass>(method, parameters3, quick);
}


std::cout << " -- Configuring the program key binds... --\n" << std::endl;

// The key binds for giving the commands.
std::array<int, 4> keybinds = LoadKeyBinds(quick);

// Load videos
LoadFiles Files(filepath);
std::vector<std::string> files = LoadVideos(Files);

// Change to the recently created save directory.
fs::current_path("results/");
fs::current_path(save_directory);
fs::copy_file("../../config/parameters.yaml", "parameters.yaml");

if (method == "EdgeDetection") {
	EdgeDetection(parameters1->get_all_parameters(), keybinds, files); // Not optimal.
	delete parameters1;
}
else if (method == "MultiDropletTracking") {
	MultiDropletTracking(parameters2, keybinds, files);
	delete parameters2;
}
else if (method == "YoloDetection") {
	YoloDetection(parameters3, keybinds, files);
	delete parameters3;
}


std::cout << "\n -- Program terminating -- \n" << std::endl;

return 0;
}


std::array<int, 4> LoadKeyBinds(bool quick)
{
	if (quick)
	{
		std::cout << "The default key binds will be used." << std::endl;
		return { int('S'), VK_SPACE, int('E'), VK_ESCAPE };
	}

	char option;
	std::cout << "Would you prefer to use the default key binds?" << std::endl;
	std::cout << "Yes (Y/y) for default key binds, no (N/n) for custom ones: ";
	std::cin >> option;

	option = (char)std::tolower(option);

	std::set<char> options = { 'y', 'n' };

	while (options.find(option) == options.end())
	{
		std::cout << "Invalid option chosen, please choose yes (Y/y) for default keybinds or no (N/n) for custom key binds: ";
		std::cin >> option;
		option = (char)std::tolower(option);
	}

	if (option == 'y')
	{
		std::cout << "The default key binds will be used." << std::endl;
		return { int('S'), VK_SPACE, int('E'), VK_ESCAPE };
	}
	else if (option == 'n')
	{
		// Load the key binds.
		std::cout << "Custom key binds will be used." << std::endl;
	}

	std::string filename = "config/custombinds.yaml";
	YAML::Node config = YAML::LoadFile(filename);
	std::string start_bind_str = config["start"].as<std::string>(); //vKey
	std::string pause_bind_str = config["pause"].as<std::string>(); //vKey
	std::string screen_shot_bind_str = config["screen_shot"].as<std::string>(); //vKey
	std::string termination_bind_str = config["termination"].as<std::string>(); //vKey

	int start_bind;
	int pause_bind;
	int screen_shot_bind;
	int termination_bind;
	std::array<int, 4> keybinds;

	if (start_bind_str.length() == 1)
	{
		start_bind = int((char)std::toupper(start_bind_str[0])); // Doesn't matter if capital or not in config file.
	}
	else
	{
		start_bind = std::stoi(start_bind_str, 0, 16);
	}

	if (pause_bind_str.length() == 1)
	{
		pause_bind = int((char)std::toupper(pause_bind_str[0]));
	}
	else
	{
		pause_bind = std::stoi(pause_bind_str, 0, 16);
	}

	if (screen_shot_bind_str.length() == 1)
	{
		screen_shot_bind = int((char)std::toupper(screen_shot_bind_str[0]));
	}
	else
	{
		screen_shot_bind = std::stoi(screen_shot_bind_str, 0, 16);
	}

	if (termination_bind_str.length() == 1)
	{
		termination_bind = int((char)std::toupper(termination_bind_str[0]));
	}
	else
	{
		termination_bind = std::stoi(termination_bind_str, 0, 16);
	}

	keybinds = { start_bind, pause_bind, screen_shot_bind, termination_bind };

	return keybinds;
}

std::string LoadVideoFilepath(std::string method) {

	std::string filename = "config/parameters.yaml";

	YAML::Node config = YAML::LoadFile(filename);
	std::string filepath = config[method]["filepath"].as<std::string>();

	return filepath;
}

template<class T>
std::string CreateDirectory(std::string method, T* parameter_class, bool quick)
{
	std::string save_folder = "results";

	if (!fs::exists(save_folder))
	{
		std::cout << "Created folder " << "'" << save_folder << "'" << " for storing screens shots." << std::endl;
		fs::create_directory(save_folder);
	}

	fs::current_path(save_folder);
	std::string directory_name = method;
	std::vector parameters = parameter_class->get_all_parameters();

	// The given parameters form a unique directory name in which the videos will be stored.
	for (double value : parameters) {
		std::string str_value = std::to_string(value);
		str_value.erase(str_value.find_last_not_of('0') + 1, std::string::npos); // Get rid of all trailing zeros.
		str_value.erase(str_value.find_last_not_of('.') + 1, std::string::npos); // Get rid of trailing dot.
		directory_name = directory_name + "_" + str_value;
	}

	char ok = 0;
	char option;

	if (fs::create_directory(directory_name))
	{
		std::cout << "Created directory: " << "'" << directory_name << "'" << std::endl;
		std::cout << std::endl;
		ok = 1;
	}

	if (!ok && quick) {
		fs::remove_all(directory_name);
		fs::create_directory(directory_name);
		std::cout << "Re-created directory: " << "'" << directory_name << "'" << std::endl;
		std::cout << std::endl;
		ok = 1;
	}

	while (!ok)
	{
		std::cout << "Directory " << "'" << directory_name << "'" << " already exists." << std::endl;
		std::cout << "Do you want to over write (O/o) " << "'" << directory_name << "'" << ", quickly create a new " << "'" << directory_name << "'" << " (Q/q) or create a new directory with a custom name (N/n): ";
		std::cin >> option;
		std::cout << std::endl;

		option = (char)std::tolower(option);
		std::set<char> options = { 'o', 'q', 'n' };

		while (options.find(option) == options.end())
		{
			std::cout << "Invalid option given, please choose (O/o) to over write " << "'" << directory_name << "'" << " or (Q/q/N/n) to create a new file: ";
			std::cin >> option;
			std::cout << std::endl;
		}

		if (option == 'o')
		{
			fs::remove_all(directory_name);
			fs::create_directory(directory_name);
			std::cout << "Re-created directory: " << "'" << directory_name << "'" << std::endl;
			std::cout << std::endl;
			ok = 1;
		}
		else if (option == 'q')
		{
			int index = 0;
			std::string original_directory_name = directory_name;

			while (!fs::create_directory(directory_name))
			{
				index++;
				directory_name = original_directory_name + "(" + std::to_string(index) + ")";
			}
			std::cout << "Created directory: " << "'" << directory_name << "'" << std::endl;
			std::cout << std::endl;
			ok = 1;
		}
		else if (option == 'n')
		{
			std::cout << "Please give the name of the new file: ";
			std::cin >> directory_name;
			std::cout << std::endl;

			if (fs::create_directory(directory_name))
			{
				std::cout << "Created directory: " << "'" << directory_name << "'" << std::endl;
				ok = 1;
			}
		}
	}

	fs::current_path("../");

	return directory_name;
}

std::vector<std::string> LoadVideos(LoadFiles& loadfileobj)
{
	loadfileobj.search_files(loadfileobj.root_path);
	std::vector<std::string> files = loadfileobj.get_files();

	int size = files.size();
	std::cout << "\nThere are " << size << " video files(mp4, avi) in this folder: " << loadfileobj.root_path << std::endl;
	std::cout << std::endl;
	for (int i = 0; i < size; i++)
	{
		std::cout << i + 1 << ": " << files[i] << std::endl;
	}
	std::cout << std::endl;

	return files;
}

int EdgeDetection(std::vector<double> parameters, std::array<int, 4> keybinds,
	std::vector<std::string> files, bool colorVideo, bool ROI_flag)
{
	double GaussianSigmaX = parameters[0];
	double GaussianSigmaY = parameters[1];
	double CannyThreshold1 = parameters[2];
	double CannyThreshold2 = parameters[3];

	double roiTop = parameters[4];
	double roiLeft = parameters[5];
	unsigned int roiHeight = parameters[6];
	unsigned int roiWidth = parameters[7];

	int start_bind = keybinds[0];
	int pause_bind = keybinds[1];
	int screen_shot_bind = keybinds[2];
	int termination_bind = keybinds[3];

	// Choose one of the videos
	std::cout << "Select a sequence number of a video that you want to play: ";
	int video_index;
	std::cin >> video_index;
	video_index = video_index - 1;
	std::cout << std::endl;


	Vision MyVision = Vision(colorVideo, ROI_flag);

	MyVision.InitializeVideo(files[video_index]);
	MyVision.InitProcessedImageDisplay(roiTop, roiLeft, roiHeight, roiWidth);
	MyVision.InitTrackerImageDisplay(roiTop, roiLeft, roiHeight, roiWidth);
	//MyVision.InitTrackerImageDisplay();

	// play videos
	Timer timer = Timer();

	bool ellipse_init_done = false;
	while (true)
	{
		// press S/s to start play

		/*if (MyVision.getFrameIndex() == 1)
		{
			while (!detectKeyStateWaitUnpress(start_bind));
			std::cout << "Begin to play the video.\n" << std::endl;
		}
		*/

		// begin with second frame
		timer.start();

		DWORD star_time = GetTickCount();

		MyVision.AcquireImage();
		MyVision.ConvertToProcessed();

		// preprocess
		MyVision.stretchContrast();
		//MyVision.Contrast(1.0, 1.3);


		//crop the processedgrayImage
		if (ROI_flag == true)
		{
			MyVision.Crop_Processed(roiTop, roiLeft, roiHeight, roiWidth);
		}
		//MyVision.autoThreshold();

		// copy the processedgrayImage to trackerGrayImage
		MyVision.CopyProcessedToTracker();


		// display
		MyVision.DisplayImage();
		MyVision.DisplayProcessedImage();
		MyVision.DisplayTrackerImage();


		/* tracking based on edge detection */
		cv::Mat edge = MyVision.DetectEdgesOpenCV(GaussianSigmaX, GaussianSigmaY, CannyThreshold1, CannyThreshold2);
		auto ellipse = MyVision.FitEllipse(edge, true, false);

		ellipse_init_done = MyVision.ellipseTracking(ellipse_init_done);


		// flush the images
		MyVision.Flush();
		MyVision.FlushProcessedImageDisplay();
		MyVision.FlushTrackerImageDisplay();

		DWORD end_time = GetTickCount();
		cout << "FPS : " << 1000.0 / (end_time - star_time) << " frame/s." << endl;


		while (timer.elapsedMilliseconds() < MyVision.getFrameTime_ms()); //wait frame time
		timer.stop();


		//MyVision.AutoScreenShot(files[video_index]);

		// press E/e to take screenshot
		if (detectKeyStateWaitUnpress(screen_shot_bind))
		{
			std::string name = files[video_index];
			MyVision.TakeAScreenShot(name, std::to_string(MyVision.getFrameIndex()));
			std::cout << "\nScreenShot already stored\n" << std::endl;
		}

		//press Esc to exit the video
		if (detectKeyStateWaitUnpress(termination_bind))
		{
			break;
		}

		// press space to pause the play
		if (detectKeyStateWaitUnpress(pause_bind))
		{
			std::cout << "Enter space again to continue; Enter 'E/e' takescreen shot; Enter Esc to exit.\n";
			while (1)
			{
				// press E/e to take screenshot
				if (detectKeyStateWaitUnpress(screen_shot_bind))
				{
					std::string name = files[video_index];
					MyVision.TakeAScreenShot(name, std::to_string(MyVision.getFrameIndex()));
					std::cout << "\nScreenShot already stored\n" << std::endl;
				}
				//press Esc to exit the video
				if (detectKeyStateWaitUnpress(termination_bind))
				{
					return 0;
				}
				// press space to continue the playing
				if (detectKeyStateWaitUnpress(pause_bind))
				{
					std::cout << "Continue to play the video.\n" << std::endl;
					break;
				}
			}
		}


		// check if the video is end
		if (MyVision.isVideoEnd())
		{
			MyVision.SaveEllipseParas(files[video_index]);
			break;
		}
	}
}

int MultiDropletTracking(MultiTrackingClass* parameters, std::array<int, 4> keybinds,
	std::vector<std::string> files)
{
	int start_bind = keybinds[0];
	int pause_bind = keybinds[1];
	int screen_shot_bind = keybinds[2];
	int termination_bind = keybinds[3];

	// Choose one of the videos
	std::cout << "Select a sequence number of a video that you want to play: ";
	int video_index;
	std::cin >> video_index;
	video_index = video_index - 1;

	double roiTop = parameters->get_roiTop();
	double roiLeft = parameters->get_roiLeft();
	unsigned int roiHeight = parameters->get_roiHeight();
	unsigned int roiWidth = parameters->get_roiWidth();
	bool selectroi = parameters->get_selectroi();
	bool treshold = parameters->get_treshold();
	bool getclicks = 0;
	bool YOLOinit = parameters->get_YOLOinit();
	bool YOLOdetect = parameters->get_YOLOdetect();

	bool colorVideo = false;
	Vision MyVision = Vision(colorVideo);
	MyVision.selectroi = selectroi;
	MyVision.treshold = treshold;
	MyVision.YOLOinit = YOLOinit;
	MyVision.YOLOdetect = YOLOdetect;

	//MyVision.InitializeVideo(files[video_index]);
	MyVision.InitializeVideo(files[video_index], roiTop, roiLeft, roiHeight, roiWidth);
	MyVision.InitProcessedImageDisplay();

	// play videos
	Timer timer = Timer();
	double accumulated_time = 0;

	// Containers for all of the point coordinates.
	std::vector<std::vector<double>> pointsx;
	std::vector<std::vector<double>> pointsy;
	std::vector<double> tempx_prev;
	std::vector<double> tempy_prev;

	double min_step = 1.0;
	double max_step = 2.5;

	std::vector<vpImagePoint> points;
	vpImagePoint point;

	int i = 0; // For indexing a frame.
	int j = 0; // For indexing tracked points in a frame.
	int k = 0; // For keeping the index of the tracked points consistent.
	int k_min;
	int first_row; // To keep the vector size consistent.
	double diff_x;
	double diff_y;
	double diff;
	double diff_min;

	YoloDetectionClass* other_parameters;

	if (YOLOinit || YOLOdetect) {
		std::filesystem::path org_path = fs::current_path();
		fs::current_path("../../");
		other_parameters = LoadParametersYoloDetection();
		fs::current_path(org_path);
	}

	while (true)
	{
		// press S/s to start play
		/*
		if (MyVision.getFrameIndex() == 1)
		{
			std::cout << "\nThis is the first frame, please press 'S/s' to begin to play the video.\n";
			//while (!detectKeyStateWaitUnpress(start_bind));
			std::cout << "Begin to play the video.\n" << std::endl;
		}
		*/



		// begin with second frame
		timer.start();
		//-------------------------------- Call test functions here-----------------------------------------------

		if (YOLOinit || YOLOdetect) {
			points = Test_MultiDropletTracking(MyVision, parameters, other_parameters);
		}
		else {
			points = Test_MultiDropletTracking(MyVision, parameters);
		}

		if (i == 0) {
			first_row = points.size();
		}



		std::vector<double> tempx(first_row + 1, 0.0);
		std::vector<double> tempy(first_row + 1, 0.0);

		tempx[0] = accumulated_time;
		tempy[0] = accumulated_time;

		//std::cout << MyVision.getFrameIndex() << std::endl;

		for (j = 0; j < points.size(); j++) {

			point = points[j];

			std::cout << point.get_i() << " " << point.get_j() << std::endl;

			if (i > 0) {
				diff_x = point.get_i() - tempx_prev[j + 1];
				diff_y = point.get_j() - tempy_prev[j + 1];
				diff = sqrt(diff_x * diff_x + diff_y * diff_y);
			}

			if (i == 0) {
				tempx[j + 1] = point.get_i();
				tempy[j + 1] = point.get_j();
			}
			else if (diff <= min_step) {
				tempx[j + 1] = point.get_i();
				tempy[j + 1] = point.get_j();
			}
			else {

				diff_min = 1000;

				for (k = 1; k <= first_row; k++) {

					if (tempx[k] != 0) continue;

					diff_x = point.get_i() - tempx_prev[k];
					diff_y = point.get_j() - tempy_prev[k];
					diff = sqrt(diff_x * diff_x + diff_y * diff_y);


					if (diff < diff_min) {
						diff_min = diff;
						k_min = k;
					}
				}

				if (diff_min <= max_step) {
					// Still in the acceptable range.
					tempx[k_min] = point.get_i();
					tempy[k_min] = point.get_j();
				}
				else {
					// Find a zero column, and start filling the values there.
					for (k = 1; k <= first_row; k++) {
						if (tempx[k] == 0) {
							tempx[k] = point.get_i();
							tempy[k] = point.get_j();
							break;
						}
					}
				}

			}
		}

		std::cout << std::endl;

		i++;

		pointsx.push_back(tempx);
		pointsy.push_back(tempy);

		tempx_prev = tempx;
		tempy_prev = tempy;


//-------------------------- The code following is common to all tests------------------------------------

		MyVision.Flush();
		MyVision.FlushProcessedImageDisplay();

		while (timer.elapsedMilliseconds() < MyVision.getFrameTime_ms()); //wait frame time
		accumulated_time = accumulated_time + timer.elapsedMilliseconds();
		timer.stop();

		// press E/e to take screenshot
		if (detectKeyStateWaitUnpress(screen_shot_bind))
		{
			std::string name = files[video_index];
			MyVision.TakeAScreenShot(name, std::to_string(MyVision.getFrameIndex()));
			std::cout << "\nScreenShot already stored\n" << std::endl;
		}

		//press Esc to exit the video
		if (detectKeyStateWaitUnpress(termination_bind))
		{
			break;
		}

		// press space to pause the play
		if (detectKeyStateWaitUnpress(pause_bind))
		{
			std::cout << "Enter space again to continue; Enter 'E/e' takescreen shot; Enter Esc to exit.\n";
			while (1)
			{
				// press E/e to take screenshot
				if (detectKeyStateWaitUnpress(screen_shot_bind))
				{
					std::string name = files[video_index];
					MyVision.TakeAScreenShot(name, std::to_string(MyVision.getFrameIndex()));
					std::cout << "\nScreenShot already stored\n" << std::endl;
				}
				//press Esc to exit the video
				if (detectKeyStateWaitUnpress(termination_bind))
				{
					return 0;
				}
				// press space to continue the playing
				if (detectKeyStateWaitUnpress(pause_bind))
				{
					std::cout << "Continue to play the video.\n" << std::endl;
					break;
				}
			}
		}


		// check if the video is end
		if (MyVision.isVideoEnd())
		{
			break;
		}
	}

	std::string m = "coordinates.mat";
	SaveMatlabMat(pointsx, pointsy, m);

	if (YOLOinit || YOLOdetect) {
		delete other_parameters;
	}

	return 0;
}

int YoloDetection(YoloDetectionClass* parameters, std::array<int, 4> keybinds,
	std::vector<std::string> files, bool colorVideo)
{

	std::string yoloConfigPath = parameters->get_yoloConfigPath();
	std::string modelParametersPath = parameters->get_modelParametersPath();
	int start_bind = keybinds[0];
	int pause_bind = keybinds[1];
	int screen_shot_bind = keybinds[2];
	int termination_bind = keybinds[3];

	std::cout << "Select a sequence number of a video that you want to play: ";
	int video_index;
	std::cin >> video_index;
	video_index = video_index - 1;
	std::cout << std::endl;

	Vision MyVision = Vision(colorVideo);
	MyVision.InitializeVideo(files[video_index]);

	bool yolo_init = false;
	// play videos
	Timer timer = Timer();
	while (true)
	{
		// press S/s to start play
		/*if (MyVision.getFrameIndex() == 1)
		{
			while (!detectKeyStateWaitUnpress(start_bind));
			std::cout << "Begin to play the video.\n" << std::endl;
		}*/

		// begin with second frame
		timer.start();

		DWORD star_time = GetTickCount();

		MyVision.AcquireImage();
		MyVision.DisplayImage();

		yolo_init = MyVision.Yolo(yolo_init, yoloConfigPath, modelParametersPath);

		// flush the images
		MyVision.Flush();

		DWORD end_time = GetTickCount();
		cout << "FPS : " << 1000.0 / (end_time - star_time) << "frame/s." << endl;

		while (timer.elapsedMilliseconds() < MyVision.getFrameTime_ms()); //wait frame time
		timer.stop();

		// press E/e to take screenshot
		if (detectKeyStateWaitUnpress(screen_shot_bind))
		{
			std::string name = files[video_index];
			MyVision.TakeAScreenShotYolo(name, std::to_string(MyVision.getFrameIndex()));
			std::cout << "\nScreenShot already stored\n" << std::endl;
		}

		//press Esc to exit the video
		if (detectKeyStateWaitUnpress(termination_bind))
		{
			break;
		}

		// press space to pause the play
		if (detectKeyStateWaitUnpress(pause_bind))
		{
			std::cout << "Enter space again to continue; Enter 'E/e' takescreen shot; Enter Esc to exit.\n";
			while (1)
			{
				// press E/e to take screenshot
				if (detectKeyStateWaitUnpress(screen_shot_bind))
				{
					std::string name = files[video_index];
					MyVision.TakeAScreenShotYolo(name, std::to_string(MyVision.getFrameIndex()));
					std::cout << "\nScreenShot already stored\n" << std::endl;
				}
				//press Esc to exit the video
				if (detectKeyStateWaitUnpress(termination_bind))
				{
					return 0;
				}
				// press space to continue the playing
				if (detectKeyStateWaitUnpress(pause_bind))
				{
					std::cout << "Continue to play the video.\n" << std::endl;
					break;
				}
			}
		}
		if (MyVision.isVideoEnd())
		{
			break;
		}

	}
	return 0;
}


