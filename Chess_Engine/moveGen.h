#pragma once

#include "position.h"
#include <map>

class MoveGen
{
private:
	Position& pos;
	std::vector<Move> moves;

	u64 player = 0ULL;
	u64 rival = 0ULL;
	u64 occupied = 0ULL;
	u64 empty = 0ULL;

	u64 pinned = 0ULL;
	std::map<Square, u64> pinDirections;
	u64 checkResponses = 0ULL;
	u64 checkers = 0ULL;
	

public:
	MoveGen(Position& position);

	u64 guarded = 0ULL;
	int checkersCount = 0;

	std::vector<Move> genMoves(bool onlyCaptures = false);
	long long perft(int depth);

private:
	bool onlyCapturesEnabled = false;

	int mod(int a, int b) { return a >= 0 ? a % b : (b - abs(a % b)) % b; }

	template <Direction D> u64 move(u64 board) {
		bool shiftLeft = D > 0;
		bool isEast = mod(D, 8) == 7; bool isDoubleEast = mod(D, 8) == 6;
		bool isWest = mod(D, 8) == 1; bool isDoubleWest = mod(D, 8) == 2;
		int magnitude = abs(D);

		if (isEast) board = board & ~FILE_H; if (isDoubleEast) board = board & ~FILE_H & ~FILE_G;
		if (isWest) board = board & ~FILE_A; if (isDoubleWest) board = board & ~FILE_A & ~FILE_B;

		return shiftLeft ? board << magnitude : board >> magnitude;
	}

	u64 white_mask();
	u64 black_mask();

	void genGuard();

	void genPawn();
	void genKnight();
	void genKing();
	void genSlidingPiece(u64 pieces, Piece pieceType);

	u64 slideLine(u64 piece, u64 line, u64 occ=NULL);
	u64 slidingAttack(Square square, Piece pieceType, u64 occ=NULL);
	u64 xRayAttack(Square square, Piece pieceType, u64 blockers);

	void appendMoves(u64 source, Direction move, char moveFlags);
	void appendPromotions(u64 source, Direction move, char moveFlags);
	void appendMoveMask(Square piece, u64 mask, char moveFlags);
	void appendGuard(Square piece, u64 mask);

	bool isSquareSafe(Square square);
	bool isSquareEmpty(Square square);
};

