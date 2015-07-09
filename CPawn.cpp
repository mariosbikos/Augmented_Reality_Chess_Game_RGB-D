#include "CPawn.h"

CPawn::CPawn(char cColor) : CPiece(cColor) 
{}

char CPawn::GetPiece()
{
	return 'P';
}