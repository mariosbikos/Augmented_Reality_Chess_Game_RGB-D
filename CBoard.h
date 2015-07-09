#ifndef CBOARD_H
#define CBOARD_H

#include <iostream>
#include "IncludeRSHeaders.h"
#include "CPiece.h"

#include "CBishop.h"
#include "CRook.h"
#include "CKnight.h"
#include "CQueen.h"
#include "CKing.h"
#include "CPawn.h"

#include "CommonOffsets.h"
//#include "Common.h"

using namespace std;

class CBoard
{
public:
	CPiece* p_PieceAtSquare[8][8];
	PXCPoint3DF32 centersOfSquares_wrt_Chessboard[8][8];

	void CBoard::SetValuesOfCenters();
	PXCPoint3DF32 GetCenterOfSquare_wrt_Chessboard(int i,int j);
	CBoard();
	~CBoard();
	void Print();
};

#endif