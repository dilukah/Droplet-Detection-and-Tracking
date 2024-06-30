#pragma once
#include <string.h>
#include <iostream>
#include <mat.h>
#include <vector>
#include <stdio.h>

using namespace std;

bool SaveMatlabMat(std::vector<std::vector<double> >& src1, std::vector<std::vector<double> >& src2, 
				   std::string file_name);

bool SaveMatlabMat(std::vector<std::vector<double> >& src1,std::string file_name);



