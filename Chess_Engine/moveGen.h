#pragma once

#include "bitboard.h"
#include "color.h"

class MoveGen
{
private:
	BitBoard &pos;
	Color turn = Color::WHITE;

public:
	MoveGen(BitBoard &position, Color turn);
	
	void genPawn();
};

