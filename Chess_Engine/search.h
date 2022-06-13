#pragma once

#include "moveGen.h"
#include "evaluation.h"
#include "transposition.h"
#include <chrono>

class Search
{
private:
	Position& pos;
	MoveGen& gen;
	Evaluation& evaluation;

	bool timerEnabled = false;
	bool onTime = false;
	std::chrono::steady_clock::time_point begin;
	int max = 2000;

	int negamax(int depth, int alpha = INT_MIN + 1, int beta = INT_MAX - 1, bool registerMove = true);
	

public:
	int nodesSearched = 0;
	int depthSearched = 0;
	Move bestMove = Move((Square) -1, (Square) - 1, 0);

	Search(Position& pos, MoveGen& gen, Evaluation& eval);

	int search(int depth);
	int quiescenceSearch(int alpha = INT_MIN + 1, int beta = INT_MAX - 1);
	void searchTimed(int milliseconds);
};

