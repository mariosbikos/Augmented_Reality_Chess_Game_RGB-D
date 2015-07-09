#ifndef CROOK_H
#define CROOK_H

#include "CPiece.h"

class CRook : public CPiece
{
public:
	CRook(char cColor) ;
	~CRook() {}
private:
	virtual char GetPiece() ;
};
#endif