#include "CChess.h"

//Constructor

CChess::CChess(): mcPlayerTurn('W')
{
	setPinchState(PinchState::FREE);
}

//setter
void CChess::setPinchState(PinchState current_hand_state)
{
	this->current_hand_state=current_hand_state;
}

//getter
PinchState CChess::getPinchState()
{
	return this->current_hand_state;
}

bool CChess::CallPlayerMove(glm::ivec2 &startSquare,glm::ivec2 &endSquare)
{
	return PlayerMove(mqGameBoard.p_PieceAtSquare,startSquare,endSquare);
}

void CChess::CallPlayEnemyMove()
{
	PlayEnemyMove(mqGameBoard.p_PieceAtSquare);
}

void CChess::CallEnemyMove()
{
	EnemyMove(mqGameBoard.p_PieceAtSquare);
}


bool CChess::PlayerMove(CPiece* qpaaBoard[8][8],glm::ivec2 StartSquare,glm::ivec2 EndSquare)
{
	using namespace std;
	QByteArray em=0;
	string combinedUserMove;
	combinedUserMove.clear();
	combinedUserMove = ConvertBoardMoveToUCIMove
		(StartSquare.x,StartSquare.y,EndSquare.x,EndSquare.y);
	combinedUserMove+="\n";
	cout<<combinedUserMove<<endl;
	//exo tin kinisi tou user se UCI format+\n
	//Prepei omos prota na kano go movetime 50 gia na do an uparxei checkmate

	exe.write(combinedUserMove.c_str());
	exe.waitForReadyRead(100);
	em= exe.readAll();
	if( em.toStdString().size()!=0)
	{
		return false;
		cout<<"Invalid Move: Try again! "<<endl;
	}
	else{
		cout<<"Valid user Move confirmed by engine"<<endl;
		if(StartSquare.x==4 && StartSquare.y==0 && EndSquare.x==6 && EndSquare.y==0)
		{
			if(qpaaBoard[StartSquare.x][StartSquare.y]->GetPiece()=='K' 
				&& qpaaBoard[StartSquare.x][StartSquare.y]->GetColor()=='W')
			{
				//user attempts a small castling
				//king goes from e1-->g1(this is done anyway since it is a move
				//But also the rook piece of h1-->f1(70-->50)
				// Make the move
				CPiece* qpTemp	 = qpaaBoard[5][0]; //get address of end square piece
				qpaaBoard[5][0]	 = qpaaBoard[7][0];
				qpaaBoard[7][0]	 = 0;
				// Make sure that the current player is not in check
				delete qpTemp;
			}
		}

		//if users attempts to move king from e1->c1(40->20) we have large castling
		if(StartSquare.x==4 && StartSquare.y==0 && EndSquare.x==2 && EndSquare.y==0)
		{
			if(qpaaBoard[StartSquare.x][StartSquare.y]->GetPiece()=='K' 
				&& qpaaBoard[StartSquare.x][StartSquare.y]->GetColor()=='W')
			{
				//user attempts a small castling
				//king goes from e1-->g1(this is done anyway since it is a move
				//But also the rook piece of a1-->d1(00-->30)
				// Make the move
				CPiece* qpTemp	 = qpaaBoard[3][0]; //get address of end square piece
				qpaaBoard[3][0]	 = qpaaBoard[0][0];
				qpaaBoard[0][0]	 = 0;
				// Make sure that the current player is not in check
				delete qpTemp;
			}
		}

		// Make the move
		CPiece* qpTemp					= qpaaBoard[EndSquare.x][EndSquare.y]; //get address of end square piece
		qpaaBoard[EndSquare.x][EndSquare.y]		= qpaaBoard[StartSquare.x][StartSquare.y];
		qpaaBoard[StartSquare.x][StartSquare.y]	= 0;
		// Make sure that the current player is not in check
		delete qpTemp;
		cout<<"Move completed in Internal Board Representation"<<endl;
		AlternateTurn();
		return true;
	}
}



void CChess::EnemyMove(CPiece* qpaaBoard[8][8])
{
	exe.write("go movetime 30\n");
	exe.waitForReadyRead(100);

	auto result6= exe.readAll();
	string st=result6.toStdString();
	string gameOverString("Game over");
	std::size_t foundEndOfGame = st.find(gameOverString);
	if (foundEndOfGame!=std::string::npos)
	{
		//USER WINS!AI can't give us a new move,so we have a check mate

		cout<<"USER WINS!!!"<<endl;
		cout<<"GAME OVER"<<endl;
		GameResult=GameWinner::USER;
	}
	else
	{
		string pass1;
		pass1.clear();
		pass1=st.substr(9,4);

		st=(st.substr(9,4))+"\n";

		//Tranform UCI String to 4 integers for iStartRow,iStartCol,iEndRow,iEndCol
		glm::uvec4 enemyMoveVec4=convertUCIMoveToBoardMove(pass1);
		int iStartRow=enemyMoveVec4.w;
		int iStartCol=enemyMoveVec4.x;
		int iEndRow=enemyMoveVec4.y;
		int iEndCol=enemyMoveVec4.z;
		cout<<"Opponents Move: "<<st<<endl;

		EnemyStartSquareCoordinates.x=iStartRow;
		EnemyStartSquareCoordinates.y=iStartCol;
		EnemyEndSquareCoordinates.x=iEndRow;
		EnemyEndSquareCoordinates.y=iEndCol;
		Start3DPositionOfEnemyPiece=mqGameBoard.GetCenterOfSquare_wrt_Chessboard(iStartRow,iStartCol);
		End3DPositionOfEnemyPiece=mqGameBoard.GetCenterOfSquare_wrt_Chessboard(iEndRow,iEndCol);
		EnemyMoveString.clear();
		EnemyMoveString=st;

	}
}

void CChess::PlayEnemyMove(CPiece* qpaaBoard[8][8])
{
	//*************************************************************//
	exe.write(EnemyMoveString.c_str());
	exe.waitForReadyRead(100);

	//e8c8(4727) enemy's big castling
	if(EnemyStartSquareCoordinates.x==4 && EnemyStartSquareCoordinates.y==7 && EnemyEndSquareCoordinates.x==2 && EnemyEndSquareCoordinates.y==7)
	{
		if(qpaaBoard[EnemyStartSquareCoordinates.x][EnemyStartSquareCoordinates.y]->GetPiece()=='K' && qpaaBoard[EnemyStartSquareCoordinates.x][EnemyStartSquareCoordinates.y]->GetColor()=='B')
		{
			//enemy attempts a castling
			//king goes from e8-->c8(this is done anyway since it is a move
			//But also the rook piece of a8-->d8(07-->37)
			// Make the move
			CPiece* qpTemp	 = qpaaBoard[3][7]; //get address of end square piece
			qpaaBoard[3][7]	 = qpaaBoard[0][7];
			qpaaBoard[0][7]	 = 0;
			// Make sure that the current player is not in check
			delete qpTemp;
		}
	}

	//e8g8(4767) enemy's small castling
	if(EnemyStartSquareCoordinates.x==4 && EnemyStartSquareCoordinates.y==7 && EnemyEndSquareCoordinates.x==6 && EnemyEndSquareCoordinates.y==7)
	{
		if(qpaaBoard[EnemyStartSquareCoordinates.x][EnemyStartSquareCoordinates.y]->GetPiece()=='K' && qpaaBoard[EnemyStartSquareCoordinates.x][EnemyStartSquareCoordinates.y]->GetColor()=='B')
		{
			//enemy attempts a castling
			//king goes from e8-->g8(this is done anyway since it is a move
			//But also the rook piece from h8-->f8(77-->57)
			// Make the move
			CPiece* qpTemp	 = qpaaBoard[5][7]; //get address of end square piece
			qpaaBoard[5][7]	 = qpaaBoard[7][7];
			qpaaBoard[7][7]	 = 0;
			// Make sure that the current player is not in check
			delete qpTemp;
		}
	}


	// Make the move
	CPiece* qpTemp					= qpaaBoard[EnemyEndSquareCoordinates.x][EnemyEndSquareCoordinates.y]; //get address of end square piece
	qpaaBoard[EnemyEndSquareCoordinates.x][EnemyEndSquareCoordinates.y]		= qpaaBoard[EnemyStartSquareCoordinates.x][EnemyStartSquareCoordinates.y];
	qpaaBoard[EnemyStartSquareCoordinates.x][EnemyStartSquareCoordinates.y]	= 0;
	// Make sure that the current player is not in check
	delete qpTemp;

	//CHECK IF USER LOSES THE GAME,AFTER THE ENEMY'S LATEST MOVE
	exe.write("go movetime 30\n");
	exe.waitForReadyRead(100);

	auto replySystem= exe.readAll();
	string stReply=replySystem.toStdString();
	string gameOverString("Game over");
	std::size_t foundEndOfGame = stReply.find(gameOverString);
	if (foundEndOfGame!=std::string::npos)
	{
		//ENEMY WINS!USER can't GIVE A new move,so we have a check mate
		cout<<"ENEMY(A.I) WINS!!!"<<endl;
		cout<<"GAME OVER"<<endl;
		GameResult=GameWinner::ENEMY;
	}

	AlternateTurn();

}



void CChess::AlternateTurn(){
	mcPlayerTurn = (mcPlayerTurn == 'W') ? 'B' : 'W';}
