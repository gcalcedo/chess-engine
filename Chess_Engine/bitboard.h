#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>

typedef unsigned long long U64;

class BitBoard
{
public:
	U64 white_pawns = 0LL;
	U64 white_rooks = 0LL;
	U64 white_knights = 0LL;
	U64 white_bishops = 0LL;
	U64 white_queens = 0LL;
	U64 white_kings = 0LL;

	U64 black_pawns = 0LL;
	U64 black_rooks = 0LL;
	U64 black_knights = 0LL;
	U64 black_bishops = 0LL;
	U64 black_queens = 0LL;
	U64 black_kings = 0LL;

	char board[8][8] =
	{
		{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
		{'-', '-', '-', '-', '-', '-', '-', '-'},
		{'-', '-', '-', '-', '-', '-', '-', '-'},
		{'-', '-', '-', '-', '-', '-', '-', '-'},
		{'-', '-', '-', '-', '-', '-', '-', '-'},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
	};

public:
	BitBoard(std::string fen);

	void init();
	void printBoard();	
	void printBitBoard(long long value);
};

