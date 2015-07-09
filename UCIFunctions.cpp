#include "UCIFunctions.h"



string ConvertBoardMoveToUCIMove(int startX,int startY, int endX,int endY) 
{
	stringstream uciPosition;

	switch (startX) {
		case 0: { uciPosition << "a"; break; }
		case 1: { uciPosition << "b"; break; }
		case 2: { uciPosition << "c"; break; }
		case 3: { uciPosition << "d"; break; }
		case 4: { uciPosition << "e"; break; }
		case 5: { uciPosition << "f"; break; }
		case 6: { uciPosition << "g"; break; }
		case 7: { uciPosition << "h"; break; }
		default: 
		{ 
			//ss=to_string(static_cast<long long> (startX));
			uciPosition << "i";
			break; 
		}
	}

	switch (startY) {
		case 0: { uciPosition << "1"; break; }
		case 1: { uciPosition << "2"; break; }
		case 2: { uciPosition << "3"; break; }
		case 3: { uciPosition << "4"; break; }
		case 4: { uciPosition << "5"; break; }
		case 5: { uciPosition << "6"; break; }
		case 6: { uciPosition << "7"; break; }
		case 7: { uciPosition << "8"; break; }
		default: 
		{ 
			uciPosition << "9";
			break; 
		}
	}

	switch (endX) {
		case 0: { uciPosition << "a"; break; }
		case 1: { uciPosition << "b"; break; }
		case 2: { uciPosition << "c"; break; }
		case 3: { uciPosition << "d"; break; }
		case 4: { uciPosition << "e"; break; }
		case 5: { uciPosition << "f"; break; }
		case 6: { uciPosition << "g"; break; }
		case 7: { uciPosition << "h"; break; }
		default: 
		{ 
			uciPosition << "i";
			break; 
		}
	}

	switch (endY) {
		case 0: { uciPosition << "1"; break; }
		case 1: { uciPosition << "2"; break; }
		case 2: { uciPosition << "3"; break; }
		case 3: { uciPosition << "4"; break; }
		case 4: { uciPosition << "5"; break; }
		case 5: { uciPosition << "6"; break; }
		case 6: { uciPosition << "7"; break; }
		case 7: { uciPosition << "8"; break; }
		default: 
		{ 
			uciPosition << "9";
			break; 
		}
	}

	return uciPosition.str();
}

glm::uvec4 convertUCIMoveToBoardMove(const string uciPosition)
{
	glm::uvec4 boardMove;
	
	switch (uciPosition[0]) {
		case 'a': { boardMove.w = 0; break; }
		case 'b': { boardMove.w = 1; break; }
		case 'c': { boardMove.w = 2; break; }
		case 'd': { boardMove.w = 3; break; }
		case 'e': { boardMove.w = 4; break; }
		case 'f': { boardMove.w = 5; break; }
		case 'g': { boardMove.w = 6; break; }
		case 'h': { boardMove.w = 7; break; }
		default : { boardMove.w =  9; break; }
	}

	switch (uciPosition[1]) {
		case '1': { boardMove.x = 0; break; }
		case '2': { boardMove.x = 1; break; }
		case '3': { boardMove.x = 2; break; }
		case '4': { boardMove.x = 3; break; }
		case '5': { boardMove.x = 4; break; }
		case '6': { boardMove.x = 5; break; }
		case '7': { boardMove.x = 6; break; }
		case '8': { boardMove.x = 7; break; }
		default : { boardMove.x =  9; break; }
	}

	switch (uciPosition[2]) {
		case 'a': { boardMove.y = 0; break; }
		case 'b': { boardMove.y = 1; break; }
		case 'c': { boardMove.y = 2; break; }
		case 'd': { boardMove.y = 3; break; }
		case 'e': { boardMove.y = 4; break; }
		case 'f': { boardMove.y = 5; break; }
		case 'g': { boardMove.y = 6; break; }
		case 'h': { boardMove.y = 7; break; }
		default : { boardMove.y =  9; break; }
	}

	switch (uciPosition[3]) {
		case '1': { boardMove.z = 0; break; }
		case '2': { boardMove.z = 1; break; }
		case '3': { boardMove.z = 2; break; }
		case '4': { boardMove.z = 3; break; }
		case '5': { boardMove.z = 4; break; }
		case '6': { boardMove.z = 5; break; }
		case '7': { boardMove.z = 6; break; }
		case '8': { boardMove.z = 7; break; }
		default : { boardMove.z =  9; break; }
	}

	return boardMove;

}



void InitializeUCInterface(QProcess &exe)
{
	exe.start("ice_full_v2_2240/ice2-x32.exe");
	exe.waitForReadyRead();
	auto result = exe.readAll();
	cout<<result.toStdString()<<endl;

	exe.write("uci\n");
	exe.waitForFinished(50);
	auto result2 = exe. readAll();
	cout<<result2.toStdString()<<endl;

	exe.write("setoption name OwnBook value false\n");
	exe.waitForReadyRead(100);
	exe.write("setoption name infoSearch value false\n");
	exe.waitForReadyRead(100);
	exe.write("setoption name infoShortStats value false\n");
	exe.waitForReadyRead(100);
	exe.write("setoption name Ponder value false\n");
	exe.waitForReadyRead(100);
	exe.write("ucinewgame\n");
	exe.waitForFinished(100);
}