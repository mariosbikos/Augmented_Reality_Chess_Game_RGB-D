// ar_chess_aruco_1.cpp : Defines the entry point for the console application.
////ARUCO+OPENGL+REALSENSE PROGRAM
//  Takes as input:
// 1)Calibration file .yml
// 2)Board configuration file .yml(exported from aruco_hrm_create_board)
// 3)Size(in meters) of a marker in the board
// 4)Out-dictionary.yml

#include <iomanip>      // std::setprecisio
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

//OpenGL Libraries
#include <windows.h>
#include <GL\GL.h>
#include <glut.h>

//Library for points etc
#include <glm.hpp>

//OpenCV Libraries
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\calib3d\calib3d.hpp"

//RealSense Libraries
#include "includeRSHeaders.h"

//ArUco Libraries
#include "aruco/aruco.h"
#include "aruco/highlyreliablemarkers.h"
#include "aruco/cvdrawingutils.h"
#include "aruco/boarddetector.h"

#include <QtCore/qt_windows.h>
#include <QtCore\QProcess>
//Visual Leak Detector library
#include <vld.h>

//Headers made by me!
#include "constants.h"

#include "KeyboardControls.h"
#include "ReadArguments.h"
#include "Settings.h"
#include "CopyStreamsToMats.h"
#include "ObjLoader.h"

#include "SetupGL.h"
#include "ResizeGL.h"
#include "IdleGL.h"
#include "Timer.h"
#include "RenderSettings.h"
#include "RenderGL.h"
#include "Common.h"
#include "CommonObjects.h"
#include "CommonOffsets.h"

using namespace std;
using namespace cv;
using namespace aruco;


int frameCnt=0;
std::vector<int> boolVec(constants::numFrames);
bool bFlagTimer=false;
bool bEndOfEnemyMoveFlag=false;;
bool bEnemyMovingPawn=false;
string EnemyMoveString="";
PXCPoint3DF32 diffvec;
PXCPoint3DF32 changingStartPoint;
//Initialize global variables
PXCPointI32 EnemyStartSquareCoordinates;
PXCPointI32 EnemyEndSquareCoordinates;
PXCPoint3DF32 Start3DPositionOfEnemyPiece;
PXCPoint3DF32 End3DPositionOfEnemyPiece;
//3D Models
std::vector< glm::vec3 > vertices_king;
std::vector< glm::vec2 > uvs_king;
std::vector< glm::vec3 > normals_king;

std::vector< glm::vec3 > vertices_queen;
std::vector< glm::vec2 > uvs_queen;
std::vector< glm::vec3 > normals_queen;

std::vector< glm::vec3 > vertices_pawn;
std::vector< glm::vec2 > uvs_pawn;
std::vector< glm::vec3 > normals_pawn;

std::vector< glm::vec3 > vertices_tower;
std::vector< glm::vec2 > uvs_tower;
std::vector< glm::vec3 > normals_tower;

std::vector< glm::vec3 > vertices_knight;
std::vector< glm::vec2 > uvs_knight;
std::vector< glm::vec3 > normals_knight;

std::vector< glm::vec3 > vertices_bishop;
std::vector< glm::vec2 > uvs_bishop;
std::vector< glm::vec3 > normals_bishop;

std::vector< glm::vec3 > vertices_board;
std::vector< glm::vec2 > uvs_board;
std::vector< glm::vec3 > normals_board;

//Debugging Input variables for .yml files and Marker Size given by User

float TheMarkerSize=-1;
string TheIntrinsicFile;
string TheBoardConfigFile;
string TheDictionaryFile;

float Offset_X_M=0.;	//from x of center of board-->x of center of left-most block
float Offset_Y_M=0.;	//from y of center of board-->y of center of bottom-most block
float Offset_Pieces_M=0.; //distance between 2 markers including space between squares 

aruco::BoardConfiguration TheBoardConfig;
aruco::CameraParameters TheDistCameraParameters, TheCameraParameters;
aruco::BoardDetector TheBoardDetector;

PXCSession *session=0;
PXCSenseManager *psm=0;
PXCBlobConfiguration* blobConfig=0;
PXCBlobData *blobData=0;

pxcU16 depth_invalid_value=0;
pxcU16 *depthValues_mm=0;
PXCProjection *projection=0;

cv::Mat ColorMat(constants::COLOR_HEIGHT,constants::COLOR_WIDTH,CV_8UC3);
cv::Mat DepthMat(constants::DEPTH_HEIGHT,constants::DEPTH_WIDTH,CV_16UC1);
cv::Mat MappedDepthMat(constants::DEPTH_HEIGHT,constants::DEPTH_WIDTH,CV_16UC1);
cv::Mat normalizedMappedDepthMat(constants::DEPTH_HEIGHT,constants::DEPTH_WIDTH,CV_32FC1) ;
cv::Mat BlobMat(constants::DEPTH_HEIGHT,constants::DEPTH_WIDTH,CV_8UC1);
cv::Mat TheInputImage_Undistorted(constants::COLOR_HEIGHT,constants::COLOR_WIDTH,CV_8UC3);
cv::Size TheGlWindowSize;

cv::Mat TheInputImage_RGB;
cv::Mat new_frame_image;

bool culling = true;
bool ccw=true;
bool wireframe=false;
bool draw_augmented_content=false;

bool flag_found_pinch_data=false;

cv::Point leftMostPoint(0,0);
cv::Point rightMostPoint(0,0);
PXCPoint3DF32 pinch_data_world_MM;

float probDetect=0.0;

bool bRenderStaticBoardFlag=false;
bool bRenderMovingPawnFlag=false;
bool bValidEngineMove=false;
//Create a GLOBAL CChess object named: game 
//and initialize its state to FREE
//CChess game;

//QProcess exe;
PXCPoint3DF32 pinch_point_wrt_chessboard_M;

glm::ivec2 SelectedStartSquare;
glm::ivec2 SelectedEndSquare;

GameWinner GameResult=GameWinner::NONE;
//0.05-10
float nearValue=0.01;
float farValue=10.0;


int main(int argc,char **argv)
{
	SelectedStartSquare.x=-1;
	SelectedStartSquare.y=-1;
	SelectedEndSquare.x=-1;
	SelectedEndSquare.y=-1;

	
	InitializeUCInterface(exe);
	try
	{
		if (readArguments (argc,argv)==false) 
		{
			cout<<"error loading arguments"<<endl;
			return -1;
		}

		SetOffsetValuesBasedOnMarkerSize();
		game.mqGameBoard.SetValuesOfCenters();
		//cout<<"Center value y: "<<game.mqGameBoard.centersOfSquares_wrt_Chessboard[1][0].x<<endl;

		cout<<"offset: "<<Offset_X_M<<endl;

		//Read board configuration from file
		TheBoardConfig.readFromFile(TheBoardConfigFile);
		// read dictionary from file and load it
		Dictionary D;
		SetDictionary(D);
		HighlyReliableMarkers::loadDictionary(D);

		//SETUP CAMERA INTRINSICS 
		SetCameraParameters(TheIntrinsicFile);
		SetBoardDetectorParameters(TheBoardConfig,TheCameraParameters,TheMarkerSize,D);

		//**************************************************************//
		cv::namedWindow("Blob Data",CV_WINDOW_AUTOSIZE);
		//cv::namedWindow("DepthMapped",CV_WINDOW_AUTOSIZE);

		//READ 1ST IMAGE FROM VIDEOCAPTURE TO GET DIMENSIONS
		session=PXCSession::CreateInstance();
		psm = PXCSenseManager::CreateInstance();
		if (!psm) 
		{
			wprintf_s(L"Unable to create the PXCSenseManager\n");
			return 1;
		}

		//Enable depth and color streams
		psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR,constants::COLOR_WIDTH,constants::COLOR_HEIGHT,constants::COLOR_FPS);
		psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, constants::DEPTH_WIDTH, constants::DEPTH_HEIGHT,constants::DEPTH_FPS); //depth resolution
		//------------------------BLOB DETECTION SETTINGS---------------------------------//
		psm->EnableBlob();
		PXCBlobModule *blobModule = psm->QueryBlob();
		// Get an instance of PXCBlobConfiguration
		blobConfig = blobModule->CreateActiveConfiguration();
		// Configure the blob module
		blobConfig->SetSegmentationSmoothing(0.1);
		blobConfig->SetMaxBlobs(1);
		blobConfig->SetContourSmoothing(0.9);
		//blobConfig->SetMinPixelCount(340000);
		//Maxobjectdepth, maxobjectdistance TO BE THE DEPTH OF CHESSBOARD---
		//when hand close to chessboard
		//blobConfig->SetMaxObjectDepth(0.1);
		//blobConfig->SetMaxDistance(0.4);
		// … call more configuration methods
		// Apply the new configuration values
		blobConfig->ApplyChanges();
		// Create an output.
		blobData = blobModule->CreateOutput();

		//Initialize Sense Manager
		if( psm -> Init() < PXC_STATUS_NO_ERROR) 
		{
			cout<<"Unable to Init the PXCSenseManager"<<endl;
			return 2;
		}
		// Create the PXCProjection instance.(ALWAYS AFTER INIT)
		PXCCapture::Device *device=psm->QueryCaptureManager()->QueryDevice();
		if(device==NULL)
		{
			cout<<"ERROR:Device not detected"<<endl;
			return 2;
		}

		depth_invalid_value=device->QueryDepthLowConfidenceValue();
		projection=device->CreateProjection();

		//GET THE 1ST FRAME TO MAKE SOME INITIALIZATIONS REGARDING SIZE ETC
		CopyImageStreamToColorMat(ColorMat);

		//Undistort the 1st given frame
		cv::undistort(ColorMat,TheInputImage_Undistorted, TheDistCameraParameters.CameraMatrix, TheDistCameraParameters.Distorsion);

		//TheInputImage_Undistorted=undistorted 1st frame image
		TheGlWindowSize=ColorMat.size();

		vResize(TheGlWindowSize.width,TheGlWindowSize.height);

		//**************************OPENGL*********************************//
		glutInit(&argc, argv);
		////Setup: Settings for rendering+Loading of 3D Models from file
		Setup();

		glutDisplayFunc( Render );
		glutIdleFunc( Idle );
		
		// Set Timer function call every 2 ms
		glutTimerFunc(2, Timer, bFlagTimer);
		glutKeyboardFunc(Keyboard);
		glutReshapeFunc( vResize );

		glutMainLoop();

	}
	catch (std::exception &ex)
	{
		cout<<"Exception :"<<ex.what()<<endl;
	}
	//deleteallocs should take place at the end of each frame capture
	//DeleteAllocs();

	system("pause");
	return 0;
}

