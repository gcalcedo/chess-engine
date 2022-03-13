#pragma once

#include "position.h"

class MoveGen
{
private:
	Position& pos;
	Color turn = Color::WHITE;

	u64 player = 0ULL;
	u64 rival = 0ULL;
	u64 occupied = 0ULL;
	u64 empty = 0ULL;

public:
	MoveGen(Position& position);

	std::vector<Move> moves;

	void genMoves(Color color);

private:

	template <Direction D> u64 move(u64 board) {
		bool shiftLeft = D > 0;
		bool isEast = D % 8 == 7;
		bool isWest = D % 8 == 1;
		int magnitude = abs(D);

		if (isEast) board = board & ~FILE_H;
		if (isWest) board = board & ~FILE_A;

		return shiftLeft ? board << magnitude : board >> magnitude;
	}

	u64 white_mask();
	u64 black_mask();

	void genPawn();
	void genKnight();
	void appendMoves(u64 source, Direction move);
	void appendPromotions(u64 source, Direction move);
};

