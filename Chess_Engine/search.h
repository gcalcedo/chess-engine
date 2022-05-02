#pragma once

#include "moveGen.h"
#include "evaluation.h"

class Search
{
private:
	Position& pos;
	MoveGen& gen;
	Evaluation& eval;

	int maxValue(int depth, int alpha = INT_MIN, int beta = INT_MAX, bool registerMove = true);
	int minValue(int depth, int alpha = INT_MIN, int beta = INT_MAX, bool registerMove = true);

public:
	Move bestMove = Move(S_A1, S_A2, 0);

	Search(Position& pos, MoveGen& gen, Evaluation& eval);

	int search(int depth, int alpha = 0, int beta = 0, bool registerMove = true);
};

