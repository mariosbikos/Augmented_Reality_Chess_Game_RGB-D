#include "PinchPointFunctions.h"


PXCPoint3DF32 GetPinchPointwrtChessboard(PXCPoint3DF32 pinch_data_world_MM )
{
	Mat T_Matrix_marker_wrt_camera ( 4,4,CV_32FC1 );
	Mat Rot ( 3,3,CV_32FC1 );
	cv::Rodrigues ( TheBoardDetector.getDetectedBoard().Rvec, Rot );
	//Extract Rot from Rvec
	//TVEC IS IN METERS
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			T_Matrix_marker_wrt_camera.at<float>(i,j)=Rot.at<float>(i,j);
		}
	}
	//					    [ Rvec00 Rvec01 Rvec02
	//T_Matrix_marker_wrt_camera =	  Rvec10 Rvec11 Rvec12
	//						  Rvec20 Rvec21 Rvec22 ]
	//
	for(int i=0;i<3;i++)
	{
		T_Matrix_marker_wrt_camera.at<float>(i,3)=TheBoardDetector.getDetectedBoard().Tvec.at<float>(i,0);
		T_Matrix_marker_wrt_camera.at<float>(3,i)=0.0;
	}
	T_Matrix_marker_wrt_camera.at<float>(3,3)=1.0;

	//								[ Rvec00 Rvec01 Rvec02 T0
	//T_Matrix_marker_wrt_camera =	  Rvec10 Rvec11 Rvec12 T1
	//								  Rvec20 Rvec21 Rvec22 T2 
	//								  0      0      0       1 ]
			
	Mat T_Matrix_camera_wrt_marker( 4,4,CV_32FC1 );
	//Matrix INVERSED!
	T_Matrix_camera_wrt_marker = T_Matrix_marker_wrt_camera.inv();
			
	//pinch_data_world_MM(meters)
	Mat pinch_point_wrt_color_camera_M ( 4,1,CV_32FC1 );

	pinch_point_wrt_color_camera_M.at<float>(0,0) = (pinch_data_world_MM.x+constants::OFFSET_RGB_DEPTH_REALSENSE_MM)/1000.0f;
	pinch_point_wrt_color_camera_M.at<float>(0,1) = -pinch_data_world_MM.y/1000.0f;
	pinch_point_wrt_color_camera_M.at<float>(0,2) = pinch_data_world_MM.z/1000.0f;
	pinch_point_wrt_color_camera_M.at<float>(0,3) = 1;
	//								  [ fingerX
	//pinch_point_wrt_color_camera_M =  -fingerY    //make it right-handed
	//									fingerZ
	//									  1 ]

	cv::Mat mat_pinch_point_wrt_chessboard_M( 4,1,CV_32FC1,cv::Scalar::all(0.0) );

	mat_pinch_point_wrt_chessboard_M = 
	T_Matrix_camera_wrt_marker * pinch_point_wrt_color_camera_M;

	PXCPoint3DF32 pinch_point_wrt_chessboard_M;
	pinch_point_wrt_chessboard_M.x=mat_pinch_point_wrt_chessboard_M.at<float>(0,0);
	pinch_point_wrt_chessboard_M.y=mat_pinch_point_wrt_chessboard_M.at<float>(1,0);
	pinch_point_wrt_chessboard_M.z=mat_pinch_point_wrt_chessboard_M.at<float>(2,0);
	return pinch_point_wrt_chessboard_M;
}
float Calculate3DDistanceOf2Points(PXCPoint3DF32 p1,PXCPoint3DF32 p2)
{
	float xd=p2.x-p1.x;
	float yd=p2.y-p1.y;
	float zd=p2.z-p1.z;
	return ( sqrt(xd*xd + yd*yd + zd*zd) );
}

float Calculate2DDistanceOf2Points(PXCPoint3DF32 p1,PXCPoint3DF32 p2)
{
	float xd=p2.x-p1.x;
	float yd=p2.y-p1.y;
	
	return ( sqrt(xd*xd + yd*yd ) );
}

float CalculateDistanceBetweenPinchPointAndCentersOfSquaresWithWhitePiece
	(PXCPoint3DF32 pinchPoint,glm::ivec2 &squareSelected)
{

	float minDistance=10000;
	int xIndex=0;
	int yIndex=0;
	float current_distance=0.0f;
	
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if( game.mqGameBoard.p_PieceAtSquare[i][j]!=0 && game.mqGameBoard.p_PieceAtSquare[i][j]->GetColor()=='W' )
			{
				current_distance=Calculate3DDistanceOf2Points(pinchPoint,game.mqGameBoard.centersOfSquares_wrt_Chessboard[i][j]);
				if(current_distance<minDistance)
				{
					minDistance=current_distance;
					xIndex=i;
					yIndex=j;
				}
			}
		}
	}
	cout<<minDistance<<endl;
	squareSelected.x=xIndex;
	squareSelected.y=yIndex;
	return minDistance;
}



float CalculateDistanceBetweenPinchPointAndPossibleEndSquare
	(PXCPoint3DF32 pinchPoint,glm::ivec2 &EndSquareSelected)
{
	float minDistance=10000;
	int xIndex=0;
	int yIndex=0;
	float current_distance=0.0f;
	
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			current_distance=Calculate2DDistanceOf2Points(pinchPoint,game.mqGameBoard.centersOfSquares_wrt_Chessboard[i][j]);
			if(current_distance<minDistance)
			{
				minDistance=current_distance;
				xIndex=i;
				yIndex=j;
			}
		}
	}
	
	
	EndSquareSelected.x=xIndex;
	EndSquareSelected.y=yIndex;
	return minDistance;


}


