#pragma once

#include "types.h"
#include "boardMask.h"
#include <random>
#include <bitset>
#include <unordered_map>


struct TranspositionEntry {
public:
	Move move;
	u64 zobristKey;
	NodeType type;
	int eval;
	int depth;

	TranspositionEntry() {
		move = Move();
		zobristKey = -1;
		type = EXACT;
		eval = 0;
		depth = 0;
	}
};

class Transposition
{
public:
	std::unordered_map<Square, std::unordered_map<Color, std::unordered_map<Piece, u64>>> seedMap;
	u64 blackMoves;
	std::vector<u64> castlingRights;
	std::unordered_map<File, u64> enPassantFiles;

	TranspositionEntry* table[65536];

	Transposition();
};