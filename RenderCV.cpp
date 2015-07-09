#include "RenderCV.h"


// **********************************************************************************
/*
* Summary:      Draws a colored circle on a Mat img 
*               using center as a parameter
* Parameters:   cv::Mat img : Mat image where the filledcircle will be drawn
*				cv::Point center : Center Point of the circle(pixels)
*				cv::Scalar color: Color of the filled circle
* Return:       Void(Draws in Mat)
*/
void MyFilledCircle( cv::Mat &img, cv::Point center,cv::Scalar color, int radius )
{
 int thickness = -1;
 int lineType = 4;

 circle( img,
         center,
         radius,
         color,
         thickness,
         lineType );
}
