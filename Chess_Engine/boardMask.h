#pragma once

#include "types.h"

static class BoardMask {
private:
	static File files[8];
	static Rank ranks[8];
	static Diagonal diagonals[15];
	static AntiDiagonal antiDiagonals[15];

public:
	static File file(Square square);
	static Rank rank(Square square);
	static Diagonal diagonal(Square square);
	static AntiDiagonal antiDiagonal(Square square);

	static u64 board(Square square);
	static u64 enPassant(Move move, Color playingColor);

	static Square square(u64 board);
};