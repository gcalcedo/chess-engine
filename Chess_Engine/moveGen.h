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
		bool isEast = D % 8 == 7; bool isDoubleEast = D % 8 == 6;
		bool isWest = D % 8 == 1; bool isDoubleWest = D % 8 == 2;
		int magnitude = abs(D);

		if (isEast) board = board & ~FILE_H; if (isDoubleEast) board = board & ~FILE_H & ~FILE_G;
		if (isWest) board = board & ~FILE_A; if (isDoubleWest) board = board & ~FILE_A & ~FILE_B;

		return shiftLeft ? board << magnitude : board >> magnitude;
	}

	u64 white_mask();
	u64 black_mask();

	void genPawn();
	void genKnight();
	void genRook();
	void genBishop();
	void genQueen();

	u64 rookLines(u64 piece);

	void appendMoves(u64 source, Direction move, char moveFlags);
	void appendPromotions(u64 source, Direction move, char moveFlags);
};

