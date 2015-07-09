#ifndef CPAWN_H
#define CPAWN_H


#include "CPiece.h"


class CPawn : public CPiece
{
public:
	CPawn(char cColor);
	~CPawn() {}
private:
	virtual char GetPiece();
};

#endif