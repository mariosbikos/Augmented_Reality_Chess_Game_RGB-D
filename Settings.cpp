#include "Settings.h"


void SetCameraParameters(std::string TheIntrinsicFile)
{
	
	TheDistCameraParameters.readFromXMLFile(TheIntrinsicFile);
	TheDistCameraParameters.resize(
		cv::Size(constants::COLOR_WIDTH,constants::COLOR_HEIGHT) );
	TheCameraParameters=TheDistCameraParameters;
	//The cv::Mat Distortion of TheCameraParameters become all-0
	TheCameraParameters.Distorsion.setTo( cv::Scalar::all(0) );

}

void SetDictionary(aruco::Dictionary &D)
{
	if (!D.fromFile(TheDictionaryFile)) 
	{
		cerr<<"Could not open dictionary file"<<endl;
        exit(1);
    }     
}

void SetBoardDetectorParameters(BoardConfiguration& TheBoardConfig,
	CameraParameters& TheCameraParameters,float TheMarkerSize,Dictionary& D)
{
	//Set the properties of aruco-BoardDetector
	TheBoardDetector.setYPerperdicular(false); 
	TheBoardDetector.setParams(TheBoardConfig,TheCameraParameters,TheMarkerSize);
	//Set the parameters of the INTERNAL Marker Detector
	//Threhold parameters
	//1)Size of a pixel neighborhood that is used to calculate a threshold value for the pixel: 3, 5, 7, and so on.
	//2)The constant subtracted from the mean or weighted mean
	TheBoardDetector.getMarkerDetector().setThresholdParams( constants::THRESHOLD_PARAMETER_1,constants::THRESHOLD_PARAMETER_2); // for blue-green markers, the window size has to be larger
	TheBoardDetector.getMarkerDetector().setMakerDetectorFunction(aruco::HighlyReliableMarkers::detect);
	TheBoardDetector.getMarkerDetector().setCornerRefinementMethod(aruco::MarkerDetector::LINES);
	TheBoardDetector.getMarkerDetector().setWarpSize( (D[0].n() + 2) * 8 );
	TheBoardDetector.getMarkerDetector().setMinMaxSize(0.005, 0.5);
}

void SetOffsetValuesBasedOnMarkerSize()
{
	//Set offset for rendering in the lower left corner of chessboard(center of marker 0,0)
	//3.5 is 3 cells and a half to go to (0,0) marker of chessboard
	Offset_X_M=(-3.5 * TheMarkerSize - 3.5 * constants::SPACE_BETWEEN_CELLS);
    Offset_Y_M=(-3.5 * TheMarkerSize - 3.5 * constants::SPACE_BETWEEN_CELLS);
	Offset_Pieces_M= TheMarkerSize  + constants::SPACE_BETWEEN_CELLS;
}