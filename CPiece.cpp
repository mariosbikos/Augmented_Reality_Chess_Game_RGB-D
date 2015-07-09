#include "CPiece.h"

CPiece::CPiece(char cColor) : mcColor(cColor)
{}

char CPiece::GetColor()
{
	return mcColor;
}