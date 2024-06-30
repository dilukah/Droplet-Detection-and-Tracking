#include<Vision.h>
#include <visp3/core/vpMunkres.h>
#include <visp3/core/vpColVector.h>

using namespace std;


/*
 Munkres algorithm: aims to find an optimal solution which minimizes the total cost of the assignments.
 Note: Minimizing totol cost does not mean assignment is always correct
 The detection should be robust to always detect the objects of interest.
*/


std::vector<vpImagePoint> Vision::TrackMunkres(std::vector<vpImagePoint> detectedPoints)
{
	static bool firstRun = false;
	static std::vector<vpImagePoint> trackedPoints;
	if (firstRun == false)
	{
		trackedPoints = detectedPoints;
		firstRun = true;
	}
	
	// Prepare Munkres (init cost matrix with detected ip / tracked ip distances)
	std::vector<std::vector<double> > cost_matrix(trackedPoints.size(), std::vector<double>(detectedPoints.size()));
	for (auto i = 0u; i < trackedPoints.size(); i++)
	{
		for (auto j = 0u; j < detectedPoints.size(); j++)
		{
			cost_matrix.at(i).at(j) = vpImagePoint::distance(trackedPoints.at(i), detectedPoints.at(j));
		}
	}
	
	trackedPoints.clear();
	for (const auto& [i, j] : vpMunkres::run(cost_matrix)) {
		trackedPoints.push_back(detectedPoints.at(j));
	}
	for (auto i = 0u; i < trackedPoints.size(); i++)
	{
		vpDisplay::displayText(processedgrayImage, trackedPoints.at(i).get_i(), trackedPoints.at(i).get_j(), to_string(i), vpColor::black);
	}
	return trackedPoints;
}

