#include "CBoard.h"

CBoard::CBoard()
{
	for (int i = 0; i < 8; ++i) 
	{
		for (int j = 0; j < 8; ++j) 
		{
			p_PieceAtSquare[i][j] = 0;
		}
	}
	// Allocate and place black pieces
	for (int i = 0; i < 8; ++i) {
		p_PieceAtSquare[i][6] = new CPawn('B');
	}
	p_PieceAtSquare[0][7] = new CRook('B');
	p_PieceAtSquare[1][7] = new CKnight('B');
	p_PieceAtSquare[2][7] = new CBishop('B');
	p_PieceAtSquare[3][7] = new CQueen('B');
	p_PieceAtSquare[4][7] = new CKing('B');
	p_PieceAtSquare[5][7] = new CBishop('B');
	p_PieceAtSquare[6][7] = new CKnight('B');
	p_PieceAtSquare[7][7] = new CRook('B');
	// Allocate and place white pieces
	for (int i = 0; i < 8; ++i) {
		p_PieceAtSquare[i][1] = new CPawn('W');
	}
	p_PieceAtSquare[0][0] = new CRook('W');
	p_PieceAtSquare[1][0] = new CKnight('W');
	p_PieceAtSquare[2][0] = new CBishop('W');
	p_PieceAtSquare[3][0] = new CQueen('W');
	p_PieceAtSquare[4][0] = new CKing('W');
	p_PieceAtSquare[5][0] = new CBishop('W');
	p_PieceAtSquare[6][0] = new CKnight('W');
	p_PieceAtSquare[7][0] = new CRook('W');
	//*****************************************************************//
	

}

void CBoard::SetValuesOfCenters()
{
	//Initialize centers of squares
	PXCPoint3DF32 center_coords;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			center_coords.x=0.0;
			center_coords.y=0.0;
			center_coords.z=0.0;
			//cout<<Offset_X_M<<endl;
			center_coords.x=Offset_X_M + i * Offset_Pieces_M * 1.0;
			center_coords.y=Offset_Y_M + j * Offset_Pieces_M * 1.0;
			center_coords.z=0.0;
			centersOfSquares_wrt_Chessboard[i][j]=center_coords;
		}
	}
}
PXCPoint3DF32 CBoard::GetCenterOfSquare_wrt_Chessboard(int i,int j)
{
	PXCPoint3DF32 center_pt;
	center_pt.x=centersOfSquares_wrt_Chessboard[i][j].x;
	center_pt.y=centersOfSquares_wrt_Chessboard[i][j].y;
	center_pt.z=centersOfSquares_wrt_Chessboard[i][j].z;
	return center_pt;
}

CBoard::~CBoard()
{
	for (int i = 0; i < 8; ++i) 
	{
		for (int j = 0; j < 8; ++j) 
		{
			delete p_PieceAtSquare[i][j];
			p_PieceAtSquare[i][j] = 0;
		}
	}
}

void CBoard::Print() 
{
	using namespace std;
	for (int j=0;j<8;j++)
	{
		for(int i=0;i<8; i++)
		{
			if( p_PieceAtSquare[i][7-j]!=0 )
			{
				cout<<p_PieceAtSquare[i][7-j]->GetColor();
				cout<<p_PieceAtSquare[i][7-j]->GetPiece();
				cout<<"|";
			}
			else cout<<"   ";
		}
		cout<<endl;
	}
}
