#ifndef CQUEEN_H
#define CQUEEN_H

#include "CPiece.h"

class CQueen : public CPiece
{
public:
	CQueen(char cColor);
	~CQueen() {}
private:
	virtual char GetPiece();
};


#endif