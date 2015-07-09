#ifndef UCIFUNCTIONS_H
#define UCIFUNCTIONS_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <glm.hpp>
#include <QtCore/qt_windows.h>
#include <QtCore\QProcess>

using namespace std;

string ConvertBoardMoveToUCIMove(int startX,int startY, int endX,int endY);
glm::uvec4 convertUCIMoveToBoardMove(const string uciPosition);
void InitializeUCInterface(QProcess &exe);

#endif
