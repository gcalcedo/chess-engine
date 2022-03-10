#pragma once

#include "position.h"

class MoveGen
{
private:
	Position& pos;
	Color turn = Color::WHITE;

	U64 player = 0ULL;
	U64 rival = 0ULL;
	U64 occupied = 0ULL;
	U64 empty = 0ULL;

public:
	MoveGen(Position& position);

	void genMoves(Color color);

private:

	template <Direction D> U64 move(U64 board) {
		bool shiftLeft = D > 0;
		bool isEast = D % 8 == 7;
		bool isWest = D % 8 == 1;
		int magnitude = abs(D);

		if (isEast) board = board & ~FILE_H;
		if (isWest) board = board & ~FILE_A;

		return shiftLeft ? board << magnitude : board >> magnitude;
	}

	U64 white_mask();
	U64 black_mask();

	void genPawn();
};

