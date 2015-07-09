#ifndef CBISHOP_H
#define CBISHOP_H

#include "CPiece.h"

class CBishop : public CPiece
{
public:
	CBishop(char cColor);
	~CBishop() {}
private:
	virtual char GetPiece();
};


#endif