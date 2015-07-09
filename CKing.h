#ifndef CKING_H
#define CKING_H

#include "CPiece.h"

class CKing : public CPiece
{
public:
	CKing(char cColor);
	~CKing() {}
private:
	virtual char GetPiece();
};


#endif