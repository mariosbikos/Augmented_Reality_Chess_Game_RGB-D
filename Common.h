#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <aruco\board.h>
#include <aruco\boarddetector.h>
#include "Constants.h"
#include "IncludeRSHeaders.h"
#include "Enums.h"
#include <glm.hpp>
#include <QtCore/qt_windows.h>
#include <QtCore\QProcess>
#include "CChess.h"

//Variables for robustness of system 
extern int frameCnt;
extern std::vector<int> boolVec;

//Animation Gloval variables
extern bool bFlagTimer;

extern PXCPoint3DF32 changingStartPoint;
extern PXCPoint3DF32 diffvec;
extern string EnemyMoveString;
extern PXCPoint3DF32 Start3DPositionOfEnemyPiece;
extern PXCPoint3DF32 End3DPositionOfEnemyPiece;
extern PXCPointI32 EnemyStartSquareCoordinates;
extern PXCPointI32 EnemyEndSquareCoordinates;
extern bool bEnemyMovingPawn;
extern bool bEndOfEnemyMoveFlag;

extern float nearValue;
extern float farValue;

//3D Models
extern std::vector< glm::vec3 > vertices_king;
extern std::vector< glm::vec2 > uvs_king;
extern std::vector< glm::vec3 > normals_king;

extern std::vector< glm::vec3 > vertices_queen;
extern std::vector< glm::vec2 > uvs_queen;
extern std::vector< glm::vec3 > normals_queen;

extern std::vector< glm::vec3 > vertices_pawn;
extern std::vector< glm::vec2 > uvs_pawn;
extern std::vector< glm::vec3 > normals_pawn;

extern std::vector< glm::vec3 > vertices_tower;
extern std::vector< glm::vec2 > uvs_tower;
extern std::vector< glm::vec3 > normals_tower;

extern std::vector< glm::vec3 > vertices_knight;
extern std::vector< glm::vec2 > uvs_knight;
extern std::vector< glm::vec3 > normals_knight;

extern std::vector< glm::vec3 > vertices_bishop;
extern std::vector< glm::vec2 > uvs_bishop;
extern std::vector< glm::vec3 > normals_bishop;

extern std::vector< glm::vec3 > vertices_board;
extern std::vector< glm::vec2 > uvs_board;
extern std::vector< glm::vec3 > normals_board;

//EDO OLES OI GLOBAL METAVLITES!



extern float TheMarkerSize;
extern std::string TheIntrinsicFile;
extern std::string TheBoardConfigFile;
extern std::string TheDictionaryFile;


extern aruco::BoardConfiguration TheBoardConfig;
extern aruco::CameraParameters TheDistCameraParameters, TheCameraParameters;
extern aruco::BoardDetector TheBoardDetector;

extern PXCSession *session;
extern PXCSenseManager *psm;
extern PXCProjection *projection;
extern PXCBlobConfiguration *blobConfig;
extern PXCBlobData* blobData;

extern pxcU16 depth_invalid_value;
extern pxcU16 *depthValues_mm;

extern cv::Mat ColorMat;
extern cv::Mat DepthMat;
extern cv::Mat MappedDepthMat;
extern cv::Mat normalizedMappedDepthMat ;
extern cv::Mat BlobMat;

extern cv::Size TheGlWindowSize;
extern cv::Mat TheInputImage_Undistorted;
extern cv::Mat TheInputImage_RGB;
extern cv::Mat new_frame_image;

extern bool culling;
extern bool ccw;
extern bool wireframe;
extern bool draw_augmented_content;

extern bool flag_found_pinch_data;
extern cv::Point leftMostPoint;
extern cv::Point rightMostPoint;

extern PXCPoint3DF32 pinch_data_world_MM;

extern float probDetect;

extern QProcess exe;

extern bool bRenderStaticBoardFlag;
extern bool bRenderMovingPawnFlag;
extern bool bValidEngineMove;
extern PXCPoint3DF32 pinch_point_wrt_chessboard_M;

//extern int selectedStartSquare_x;
//extern int selectedStartSquare_y;

extern glm::ivec2 SelectedStartSquare;
extern glm::ivec2 SelectedEndSquare;

extern GameWinner GameResult;
#endif