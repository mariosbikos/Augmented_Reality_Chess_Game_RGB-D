#ifndef RENDER_GL_H
#define RENDER_GL_H

#include <windows.h>
#include <GL\GL.h>
#include <glut.h>
#include "Common.h"
#include "CommonObjects.h"
#include "Constants.h"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Enums.h"
#include "RenderSettings.h"
#include "CommonOffsets.h"


GLuint matToTexture(cv::Mat &mat);
void Render();
void axis(float size);
void DrawVideoStreamInOpenGLWindow();
void drawCube();
void DrawBoardExceptForSelectedENEMYMovingPiece(CChess &gameref);
void DrawPieceofColorAndTypeInPosition(char chessPieceType,char colorOfPiece,int x, int y);
void DrawEnemyMovingPiece(char chessPieceType,char colorOfPiece);
void DrawStaticChessPieces(CChess &gameref);
void DrawPinchPoint3D();
void DrawProjectionOfPinchPoint3D();
void DrawBoardExceptForSelectedMovingPiece(CChess &gameref);
void DrawMovingPiece(char chessPieceType,char colorOfPiece,PXCPoint3DF32 pinchPoint);
void DrawChessBoard();
void DrawString(char *string);



#endif