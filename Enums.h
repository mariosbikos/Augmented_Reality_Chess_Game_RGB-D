#ifndef ENUMS_H
#define ENUMS_H

typedef enum {
    BLACK_PLASTIC=1,
    WHITE=2,
	GOLD=3,
	RUBY,
	RED
} PieceMaterial;

typedef enum {
    FREE=0,
    PINCH_IN=1,
	PINCH_CONTINUOUS=2,
	PINCH_OUT=3,
	ENEMY_MOVE=4
} PinchState;

typedef enum{
	NONE=0,
	USER=1,
	ENEMY=2
}GameWinner;

#endif