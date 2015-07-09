#ifndef CCHESS_H
#define CCHESS_H

#include "Enums.h"
#include "Common.h"
#include "CBoard.h"
#include "UCIFunctions.h"

class CChess
{
public:
	CBoard mqGameBoard;
	PinchState current_hand_state;
	char mcPlayerTurn;

	//constructors
	CChess();
	~CChess() {}
	//methods
	void setPinchState(PinchState current_hand_state);
	PinchState getPinchState();
	bool CallPlayerMove(glm::ivec2 &startSquare,glm::ivec2 &endSquare);
	bool PlayerMove(CPiece* qpaaBoard[8][8],glm::ivec2 StartSquare,glm::ivec2 EndSquare) ;
	void CallEnemyMove();
	void CallPlayEnemyMove();
	void EnemyMove(CPiece* qpaaBoard[8][8]);
	void PlayEnemyMove(CPiece* qpaaBoard[8][8]);
	void AlternateTurn();
};

#endif