#ifndef CONTOUR_FUNCTIONS_H
#define CONTOUR_FUNCTIONS_H

#include "Constants.h"
#include "Common.h"
#include "opencv2/core/core.hpp"
#include "includeRSHeaders.h"
#include "LineFunctions.h"
#include <iostream>
using namespace std;

void FindLeftMostAndRightMostPointsOfInnerContour(
	vector<PXCPointI32> &contours_vec,
	cv::Point &leftMostPoint,
	cv::Point &rightMostPoint
);

void InitializeLeftandRightMostPoints(cv::Point &leftMostPoint,cv::Point &rightMostPoint);

void CreatePinchNeighborhoodPoints(cv::Point &leftMostPoint,cv::Point &rightMostPoint,vector<cv::Point> &target_points_neighborhood_vec,int sizeOfNeighborhood);

float GetAverageZValueOfNeighborhoodPoints(
	vector<cv::Point> &target_points_neighborhood_v);

void GetAverageXandYofMappedPoints(int &avg_x_of_mapped_points,int &avg_y_of_mapped_points,vector<cv::Point> &target_points_neighborhood_v,PXCPointF32* uvmap);


#endif