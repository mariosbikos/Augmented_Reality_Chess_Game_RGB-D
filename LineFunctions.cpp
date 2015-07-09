#include "LineFunctions.h"


// **********************************************************************************
/*
* Summary:      Returns y(int) position of a pixel point in line ab,
*               given the x pixel-coordinate of the point
* Parameters:   cv::Point a : 1st Point of Line
*				cv::Point b : 2nd Point of Line
*				int x :	The x pixel-coordinate of the point
* Return:       (int)The y pixel position 
*/
int GetYofLineabGivenX(cv::Point a, cv::Point b,int x)
{
	double slope=(double)(b.y-a.y)/(b.x-a.x);
	return (int)((slope*(x-a.x)) + a.y);
}