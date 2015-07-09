#ifndef IDLE_GL_H
#define IDLE_GL_H

#include <iomanip>
#include <iostream>
#include <windows.h>
#include <GL\GL.h>
#include <glut.h>
//OpenCV Libraries
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\calib3d\calib3d.hpp"
#include "KeyboardControls.h"
#include "IncludeRSHeaders.h"
#include "CopyStreamsToMats.h"
#include "ContourFunctions.h"
#include "RenderCV.h"
#include "Enums.h"
#include "CChess.h"
#include "Common.h"
#include "CommonObjects.h"
#include "ObjLoader.h"
#include "PinchPointFunctions.h"
#include "Constants.h"
#include <numeric>
using namespace std;


void Idle();

#endif