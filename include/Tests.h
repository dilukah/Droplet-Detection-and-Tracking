#pragma once

#include "Vision.h"
#include "Auxiliary.h"


std::vector<vpImagePoint> Test_MultiDropletTracking(Vision& MyVision, MultiTrackingClass* parameters);

std::vector<vpImagePoint> Test_MultiDropletTracking(Vision& MyVision, MultiTrackingClass* parameters, YoloDetectionClass* other_parameters);
