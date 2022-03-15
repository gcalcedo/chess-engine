#pragma once

#include "types.h"

static class BoardMapper {
private:
	static File files[8];
	static Rank ranks[8];
	static Diagonal diagonals[15];
	static AntiDiagonal antiDiagonals[15];

public:
	static File getFile(Square square);
	static Rank getRank(Square square);
	static Diagonal getDiagonal(Square square);
	static AntiDiagonal getAntiDiagonal(Square square);
};