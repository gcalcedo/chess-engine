#pragma once

#include "position.h"
#include "moveGen.h"
#include "boardMask.h"

class Evaluation
{
private:
	Position& pos;
	MoveGen& gen;
	const int KNIGHT_VALUE = 300;
	const int BISHOP_VALUE = 300;
	const int ROOK_VALUE = 500;
	const int QUEEN_VALUE = 900;
	const int PAWN_VALUE = 100;

	int pieceValue(Piece piece);
	int materialCount(Color color);

public:
	Evaluation(Position& pos, MoveGen& gen);

	std::vector<Move> sortMoves(std::vector<Move> moves, Move hashMove);
	int evaluate();
};

