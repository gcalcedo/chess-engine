#pragma once

#include "position.h"
#include "moveGen.h"
#include "boardMask.h"
#include <unordered_map>

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

	std::unordered_map<Color, std::unordered_map<Piece, std::vector<int>>> pieceSquareTable;

	int pieceValue(Piece piece);
	int materialCount(Color color);
	int pieceSquareEval(Color color);

public:
	Evaluation(Position& pos, MoveGen& gen);

	std::vector<Move> sortMoves(std::vector<Move> moves, Move hashMove);
	int evaluate();
};

