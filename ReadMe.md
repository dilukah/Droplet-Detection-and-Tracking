# Liquid droplet detection and tracking

## Table of Contents

- Introduction
- Features
- Installation
- Authors

## Introduction

This project is designed to address several key challenges in liquid detection and tracking. By leveraging classical machine vision algorithms and AI algorithms, it aims to provide solutions that improve detection and tracking accuracy, processing speed. Specifically, this project tackles the following problems:

1. **Detection and Tracking Accuracy of Droplets:** Reliable detection of objects or features within the video is crucial for accurate analysis. This project includes robust algorithms and methodologies to improve detection accuracy, ensuring precise identification of the droplet. This project implements advanced tracking techniques to maintain high accuracy.
2. **Slow Processing Times:** Traditional data processing methods can be slow and inefficient. By implementing algorithms in C++, we significantly reduce processing times without compromising accuracy.
3. **Cumbersome Configuration:** Setting up image processing parameters can be tedious and error-prone. This project includes a configuration file that allows for easy and flexible parameter adjustments, streamlining the setup process.

## Features

- Single droplet detection and tracking 
- Droplet rotation tracking (https://doi.org/10.1021/acs.langmuir.3c00298, Figure 3A)
- Multi droplet tracking with Yolov4-Deepsort
- Multi droplet tracking with Munkres algorithm
- Matlab Export

## Installation

Instructions to execute the liquid tracking project.

1. Compile the project using Cmake. 

2. Open the project, change "Solution Configurations" and "Solution Platforms" into "Release" and "x64", respectively.

3. Open the property menu of this project in "Release | x64" mode to configure the matlab in the following way:
	1) Open the property menu of this project in "Release | x64" mode, select C/C++ ---> General ---> Additional include Directories, and add the following path:
		[path of matlab]\extern\include
		[path of matlab]\extern\include\win64

	2) Open the property menu of this project in "Release | x64" mode, select Linker ---> General ---> Additional library Directories, and add the following path:
		[path of matlab]\extern\lib\win64\microsoft

	3) Open the property menu of this project in "Release | x64" mode, select Linker ---> Input ---> Additional Dependencies, and add the following files:
		libmat.lib
		libmax.lib
		libmex.lib

	4) Minimize your project window and right click "the computer" in the main surface, select 
		Properties --->Advanced System Settings ---> Environment Variables ---> Path ---> Edit
	    then add the new path to your computer:
		[path of matlab]\bin\win64
  Note the recommended Matlab version is 2020b or other new versions. 

4. Open the project window again and compile this project to generate executable file.

## Authors
 
- Diluka Harischandra (Instructor, Initial work)
- Esa Paavilainen (Additional features)
- Yuhang Yang (Additional features)
- Yinda Xu (Additional features)