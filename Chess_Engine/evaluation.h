#pragma once

#include "position.h"

class Evaluation
{
private:
	Position& pos;
	const int KNIGHT_VALUE = 3;
	const int BISHOP_VALUE = 3;
	const int ROOK_VALUE = 5;
	const int QUEEN_VALUE = 9;
	const int PAWN_VALUE = 1;

	int materialCount(Color color);

public:
	Evaluation(Position& pos);

	int evaluate();
};

