#ifndef CKNIGHT_H
#define CKNIGHT_H


#include "CPiece.h"

class CKnight : public CPiece
{
public:
	CKnight(char cColor);
	~CKnight() {}
private:
	virtual char GetPiece();
};

#endif