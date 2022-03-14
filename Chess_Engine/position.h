#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "types.h"

class Position
{
public:
	u64 white_pawns = 0LL;
	u64 white_rooks = 0LL;
	u64 white_knights = 0LL;
	u64 white_bishops = 0LL;
	u64 white_queens = 0LL;
	u64 white_kings = 0LL;

	u64 black_pawns = 0LL;
	u64 black_rooks = 0LL;
	u64 black_knights = 0LL;
	u64 black_bishops = 0LL;
	u64 black_queens = 0LL;
	u64 black_kings = 0LL;

	char board[8][8] =
	{
		{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
		{'-', '-', '-', '-', '-', '-', '-', '-'},
		{'-', '-', '-', '-', 'R', '-', '-', '-'},
		{'-', '-', '-', '-', '-', '-', '-', '-'},
		{'-', '-', '-', '-', '-', '-', '-', '-'},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{'N', 'N', 'B', 'Q', 'K', 'B', 'N', 'N'}
	};

public:
	Position(std::string fen);

	void init();
	u64 getBoard(Color color, Piece piece);
};

