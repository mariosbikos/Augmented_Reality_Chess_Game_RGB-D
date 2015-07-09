#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants
{
	
	const int numFrames=4;
	const float SPEED=1;
	const float elapsed=0.01;
	const float ABOVE_GROUND_M = 0.033;
	const float ANIMATION_Z_OFFSET=0.10;
	const float MOVING_PAWN_OFFSET = -0.02;
    //dimensions of chessboard and cells in METERS
	//0.2
	const float  BOARD_SIZE						=	0.37;
	//0.004
	const float  SPACE_BETWEEN_CELLS			=	0.008;
	//0.002
	const float  HALF_SPACE_BETWEEN_CELLS		=	0.004;
	//0.01
	const float  SCALING_FACTOR_MODEL			=	0.02;
	
	const float  DISTANCE_THRESHOLD_FOR_PIECE_SELECTION = 0.13;
	//Opencv-aruco constants
	const int    VIDEOCAPTURE_CAMERA_INDEX		=	1;
	const double THRESHOLD_PARAMETER_1			=	21;
	const double THRESHOLD_PARAMETER_2			=	7;
	
	//REALSENSE CONSTANTS
	const int COLOR_HEIGHT = 480;
	const int COLOR_WIDTH  = 640;
	const int DEPTH_HEIGHT = 480;
	const int DEPTH_WIDTH  = 640;
	const int COLOR_FPS	   = 60;
	const int DEPTH_FPS	   = 60;
	
	const int OUTER_CONTOUR_INDEX =0;
	const int INNER_CONTOUR_INDEX =1;
	const float  OFFSET_RGB_DEPTH_REALSENSE_MM		  =	  25;
	const int MIN_INNER_CONTOUR_POINTS_TO_VALID_PINCH =	  8;

} // namespace constants

#endif