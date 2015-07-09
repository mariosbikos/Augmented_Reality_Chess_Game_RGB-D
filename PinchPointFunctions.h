#ifndef PINCH_POINT_FUNCTIONS_H
#define PINCH_POINT_FUNCTIONS_H

//OpenCV Libraries
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\calib3d\calib3d.hpp"
//RealSense Libraries
#include "includeRSHeaders.h"
#include "Constants.h"
#include "Common.h"
#include "CommonObjects.h"
using namespace cv;

PXCPoint3DF32 GetPinchPointwrtChessboard(PXCPoint3DF32 pinch_data_world_MM );
float Calculate3DDistanceOf2Points(PXCPoint3DF32 p1,PXCPoint3DF32 p2);
float Calculate2DDistanceOf2Points(PXCPoint3DF32 p1,PXCPoint3DF32 p2);
float CalculateDistanceBetweenPinchPointAndCentersOfSquaresWithWhitePiece
	(PXCPoint3DF32 pinchPoint,glm::ivec2 &squareSelected);
float CalculateDistanceBetweenPinchPointAndPossibleEndSquare
	(PXCPoint3DF32 pinchPoint,glm::ivec2 &squareSelected);

#endif