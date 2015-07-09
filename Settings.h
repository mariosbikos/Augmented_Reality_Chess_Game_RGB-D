#ifndef SETTINGS_H
#define SETTINGS_H


#include "common.h"
#include "Constants.h"
#include <aruco\aruco.h>
#include <aruco/highlyreliablemarkers.h>

using namespace aruco;

void SetCameraParameters(std::string TheIntrinsicFile);
void SetDictionary(Dictionary &D);
void SetBoardDetectorParameters(BoardConfiguration& TheBoardConfig,
	CameraParameters& TheCameraParameters,float TheMarkerSize,Dictionary& D);

void SetOffsetValuesBasedOnMarkerSize();

#endif