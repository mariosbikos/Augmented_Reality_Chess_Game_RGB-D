#include "Timer.h"

void Timer(int value)
{
	if(value==true)
	{
		//cout<<fixed<<endl;
		//cout<<"Current: "<<changingStartPoint.x<<","<<changingStartPoint.y<<","<<changingStartPoint.z<<endl;
		//cout<<"End: "<<End3DPositionOfEnemyPiece.x<<","<<End3DPositionOfEnemyPiece.y<<","<<End3DPositionOfEnemyPiece.z<<endl;

		//if( (changingStartPoint.x+(constants::SPEED*diffvec.x))!=End3DPositionOfEnemyPiece.x)
		//cout<<"DISTANCE OF CURRENT TO END- SQUARE: "<<Calculate3DDistanceOf2Points(changingStartPoint,End3DPositionOfEnemyPiece)<<endl;

		if( Calculate3DDistanceOf2Points(changingStartPoint,Start3DPositionOfEnemyPiece)
			>=
			Calculate3DDistanceOf2Points(Start3DPositionOfEnemyPiece,End3DPositionOfEnemyPiece))
		{
			bFlagTimer=false;
			cout<<"EKTELO ENTOLI"<<endl;
			game.CallPlayEnemyMove();

			//to piece eftase
			//Zografise pali to board opos itan
			bRenderStaticBoardFlag=true;
			bRenderMovingPawnFlag=false;
			bValidEngineMove=false;
			
			bEnemyMovingPawn=false;
			bEndOfEnemyMoveFlag=true;
		}
		else
		{
			//UPDATE POSITION OF MOVING PAWN
			changingStartPoint.x+=(constants::SPEED*constants::elapsed*diffvec.x);
			changingStartPoint.y+=constants::SPEED*constants::elapsed*diffvec.y;
			changingStartPoint.z+=constants::SPEED*constants::elapsed*diffvec.z;
			bEndOfEnemyMoveFlag=false;
			bFlagTimer=true;
			
		}
	}
	// Recall after 200 ms
	glutTimerFunc(2, Timer, bFlagTimer);
	glutPostRedisplay();


}