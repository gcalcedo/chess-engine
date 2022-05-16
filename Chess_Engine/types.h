#pragma once

#include <string>
#include <iostream>

#include "stringify.h"

/// <summary>
/// 64 bitboard.
/// </summary>
typedef unsigned long long u64;

#pragma region Pieces
enum Color {
	WHITE = 1,
	BLACK = -1
};
const Color ColorList[] = { Color::WHITE, Color::BLACK };

enum Piece {
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	PAWN
};
const Piece PieceList[] = { KNIGHT, BISHOP, ROOK, QUEEN, KING, PAWN };

#pragma endregion

#pragma region Board
enum Direction {
	NORTH = 8,
	EAST = -1,
	SOUTH = -NORTH,
	WEST = -EAST,

	NORTH_EAST = NORTH + EAST,
	NORTH_WEST = NORTH + WEST,
	SOUTH_EAST = SOUTH + EAST,
	SOUTH_WEST = SOUTH + WEST,

	NORTH2 = NORTH + NORTH,
	EAST2 = EAST + EAST,
	SOUTH2 = SOUTH + SOUTH,
	WEST2 = WEST + WEST,

	NORTH2_EAST = NORTH2 + EAST,
	NORTH2_WEST = NORTH2 + WEST,
	SOUTH2_EAST = SOUTH2 + EAST,
	SOUTH2_WEST = SOUTH2 + WEST,
	EAST2_NORTH = EAST2 + NORTH,
	EAST2_SOUTH = EAST2 + SOUTH,
	WEST2_NORTH = WEST2 + NORTH,
	WEST2_SOUTH = WEST2 + SOUTH
};

enum File : u64 {
	FILE_A = 0x8080808080808080ULL,
	FILE_B = FILE_A >> 1,
	FILE_C = FILE_A >> 2,
	FILE_D = FILE_A >> 3,
	FILE_E = FILE_A >> 4,
	FILE_F = FILE_A >> 5,
	FILE_G = FILE_A >> 6,
	FILE_H = FILE_A >> 7
};
const File FileList[] = { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };

enum Rank : u64 {
	RANK_1 = 0x00000000000000FFULL,
	RANK_2 = RANK_1 << 8 * 1,
	RANK_3 = RANK_1 << 8 * 2,
	RANK_4 = RANK_1 << 8 * 3,
	RANK_5 = RANK_1 << 8 * 4,
	RANK_6 = RANK_1 << 8 * 5,
	RANK_7 = RANK_1 << 8 * 6,
	RANK_8 = RANK_1 << 8 * 7
};

enum Diagonal : u64 {
	DIAG_1 = 0x8000000000000000ULL,
	DIAG_2 = 0x4080000000000000ULL,
	DIAG_3 = 0x2040800000000000ULL,
	DIAG_4 = 0x1020408000000000ULL,
	DIAG_5 = 0x0810204080000000ULL,
	DIAG_6 = 0x0408102040800000ULL,
	DIAG_7 = 0x0204081020408000ULL,
	DIAG_8 = 0x0102040810204080ULL,
	DIAG_9 = 0x0001020408102040ULL,
	DIAG_10 = 0x0000010204081020ULL,
	DIAG_11 = 0x0000000102040810ULL,
	DIAG_12 = 0x0000000001020408ULL,
	DIAG_13 = 0x0000000000010204ULL,
	DIAG_14 = 0x0000000000000102ULL,
	DIAG_15 = 0x0000000000000001ULL
};

enum AntiDiagonal :u64 {
	ANTI_DIAG_1 = 0x0100000000000000ULL,
	ANTI_DIAG_2 = 0x0201000000000000ULL,
	ANTI_DIAG_3 = 0x0402010000000000ULL,
	ANTI_DIAG_4 = 0x0804020100000000ULL,
	ANTI_DIAG_5 = 0x1008040201000000ULL,
	ANTI_DIAG_6 = 0x2010080402010000ULL,
	ANTI_DIAG_7 = 0x4020100804020100ULL,
	ANTI_DIAG_8 = 0x8040201008040201ULL,
	ANTI_DIAG_9 = 0x0080402010080402ULL,
	ANTI_DIAG_10 = 0x0000804020100804ULL,
	ANTI_DIAG_11 = 0x0000008040201008ULL,
	ANTI_DIAG_12 = 0x0000000080402010ULL,
	ANTI_DIAG_13 = 0x0000000000804020ULL,
	ANTI_DIAG_14 = 0x0000000000008040ULL,
	ANTI_DIAG_15 = 0x0000000000000080ULL
};

enum Square {
	S_H1, S_G1, S_F1, S_E1, S_D1, S_C1, S_B1, S_A1,
	S_H2, S_G2, S_F2, S_E2, S_D2, S_C2, S_B2, S_A2,
	S_H3, S_G3, S_F3, S_E3, S_D3, S_C3, S_B3, S_A3,
	S_H4, S_G4, S_F4, S_E4, S_D4, S_C4, S_B4, S_A4,
	S_H5, S_G5, S_F5, S_E5, S_D5, S_C5, S_B5, S_A5,
	S_H6, S_G6, S_F6, S_E6, S_D6, S_C6, S_B6, S_A6,
	S_H7, S_G7, S_F7, S_E7, S_D7, S_C7, S_B7, S_A7,
	S_H8, S_G8, S_F8, S_E8, S_D8, S_C8, S_B8, S_A8,
};
const Square SquareList[] = {
	S_H1, S_G1, S_F1, S_E1, S_D1, S_C1, S_B1, S_A1,
	S_H2, S_G2, S_F2, S_E2, S_D2, S_C2, S_B2, S_A2,
	S_H3, S_G3, S_F3, S_E3, S_D3, S_C3, S_B3, S_A3,
	S_H4, S_G4, S_F4, S_E4, S_D4, S_C4, S_B4, S_A4,
	S_H5, S_G5, S_F5, S_E5, S_D5, S_C5, S_B5, S_A5,
	S_H6, S_G6, S_F6, S_E6, S_D6, S_C6, S_B6, S_A6,
	S_H7, S_G7, S_F7, S_E7, S_D7, S_C7, S_B7, S_A7,
	S_H8, S_G8, S_F8, S_E8, S_D8, S_C8, S_B8, S_A8
};

enum Side : u64 {
	QUEEN_SIDE = FILE_A | FILE_B | FILE_C | FILE_D,
	KING_SIDE = FILE_E | FILE_F | FILE_G | FILE_H,
	WHITE_SIDE = RANK_1 | RANK_2 | RANK_3 | RANK_4,
	BLACK_SIDE = RANK_5 | RANK_6 | RANK_7 | RANK_8
};

enum NodeType {
	EXACT,
	LOWER_BOUND,
	UPPER_BOUND
};
#pragma endregion

#pragma region Moves

typedef unsigned short u16;

enum MoveType {
	QUIET,
	DOUBLE_PAWN_PUSH = 1,
	KING_SIDE_CASTLE = 2,
	QUEEN_SIDE_CASTLE = 3,
	CAPTURE = 4,
	EN_PASSANT = 5,
	PROMOTION = 8,
};

/// <summary>
/// <para> Bits [15...10] -> FROM </para>
/// <para> Bits [9 ... 4] -> TO </para>
/// <para> Bits [3 ... 0] -> MOVE FLAGS </para>
/// </summary>
struct Move {
private:
	u16 data = 0;

public:
	Move() {}

	Move(Square from, Square to, char flags) {
		data += from << 10;
		data += to << 4;
		data += flags;
	}

	u64* capture;
	Square capturedSquare;
	char castlingRights = 0;

	int score = 0;

	//METHODS

	Square from() { return (Square)((data >> 10) & 63); }
	Square to() { return (Square)((data >> 4) & 63); }
	Piece getPromotion() { return (Piece)(data & 3); }

	bool isQuiet() { return (data & 15) == QUIET; }
	bool isDoublePawnPush() { return (data & 15) == DOUBLE_PAWN_PUSH; }
	bool isEnPassant() { return (data & 15) == EN_PASSANT; }
	bool isCapture() { return (data & 4) == CAPTURE; }
	bool isPromotion() { return (data & 8) == PROMOTION; }
	bool isKingSideCastle() { return (data & 15) == KING_SIDE_CASTLE; }
	bool isQueenSideCastle() { return (data & 15) == QUEEN_SIDE_CASTLE; }

	void print() {
		//for (size_t i = 1; i <= 16; i++) {
		//	if (i == 7 || i == 13) std::cout << " | ";
		//	if ((1ll << (16 - i)) & data) {
		//		std::cout << "1 ";
		//	}
		//	else {
		//		std::cout << "0 ";
		//	}
		//}

		std::cout //<< "-> "
			<< Stringify::square(from()) << "-"
			<< Stringify::square(to());

		if (isDoublePawnPush()) std::cout << " | Double Pawn Push";
		if (isCapture()) std::cout << " | Capture";
		if (isPromotion()) std::cout << " | Promotion [" << Stringify::piece(getPromotion()) << "]";
		if (isEnPassant()) std::cout << " | En-Passant";
		if (isKingSideCastle()) std::cout << " | King Side Castle";
		if (isQueenSideCastle()) std::cout << " | Queen Side Castle";

		std::cout << std::endl;
	}
};
#pragma endregion
