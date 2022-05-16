#include "boardMask.h"

//LOOK UP TABLES
File BoardMask::files[8] = { FILE_H, FILE_G, FILE_F, FILE_E, FILE_D, FILE_C, FILE_B, FILE_A };

Rank BoardMask::ranks[8] = { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };

Diagonal BoardMask::diagonals[15] =
{ DIAG_15, DIAG_14, DIAG_13, DIAG_12, DIAG_11, DIAG_10, DIAG_9,
  DIAG_8, DIAG_7, DIAG_6, DIAG_5, DIAG_4, DIAG_3, DIAG_2, DIAG_1 };

AntiDiagonal BoardMask::antiDiagonals[15] =
{ ANTI_DIAG_1, ANTI_DIAG_2, ANTI_DIAG_3, ANTI_DIAG_4, ANTI_DIAG_5, ANTI_DIAG_6, ANTI_DIAG_7, ANTI_DIAG_8,
  ANTI_DIAG_9, ANTI_DIAG_10, ANTI_DIAG_11, ANTI_DIAG_12, ANTI_DIAG_13, ANTI_DIAG_14, ANTI_DIAG_15 };

//MASK METHODS

File BoardMask::file(Square square) { return files[square % 8]; }

Rank BoardMask::rank(Square square) { return ranks[square / 8]; }

Diagonal BoardMask::diagonal(Square square) {
	return diagonals[(square - (square % 8)) / 8 + square - (square - (square % 8))];
};

AntiDiagonal BoardMask::antiDiagonal(Square square) {
	return antiDiagonals[(7 - (square - (square % 8)) / 8) + square - (square - (square % 8))];
}

u64 BoardMask::board(Square square) {
	return (0x1ULL << square);
}

u64 BoardMask::enPassant(Move move, Color playingColor) {
	if (playingColor == Color::WHITE) {
		return (0x1ULL << move.to()) << 8;
	}
	else {
		return (0x1ULL << move.to()) >> 8;
	}
}

Square BoardMask::square(u64 board) {
	unsigned long idx;
	_BitScanForward64(&idx, board);
	return (Square)idx;
}