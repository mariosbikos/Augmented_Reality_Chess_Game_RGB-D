#ifndef CPIECE_H
#define CPIECE_H


class CPiece
{

private:
	char mcColor;

public:
	CPiece(char cColor);

	~CPiece() {}

	virtual char GetPiece() = 0;

	char GetColor() ;
	
};

#endif