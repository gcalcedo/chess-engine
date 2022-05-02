#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "types.h"
#include <map>

class Position
{
private:
	std::vector<Move> history;
	struct CastlingRights {
		bool white_qcastle = true;
		bool white_kcastle = true;
		bool black_qcastle = true;
		bool black_kcastle = true;
	};

public:
	CastlingRights castling;
	Color turnColor = Color::WHITE;

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

	std::map<Color, std::map<Piece, u64*>> boards = {
		{Color::WHITE, {
			{PAWN, &white_pawns},
			{ROOK, &white_rooks},
			{KNIGHT, &white_knights},
			{BISHOP, &white_bishops},
			{QUEEN, &white_queens},
			{KING, &white_kings},
		}},
		{Color::BLACK, {
			{PAWN, &black_pawns},
			{ROOK, &black_rooks},
			{KNIGHT, &black_knights},
			{BISHOP, &black_bishops},
			{QUEEN, &black_queens},
			{KING, &black_kings},
		}}
	};

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
	Position(std::string fen);

	void init();
	u64 getBoard(Color color, Piece piece);

	void makeMove(Move move);
	void unMakeMove();
	int moveCount();
	Move lastMove();
	void print();

private:
	u64* boardReference(Square square);
	u64* boardReference(Color color, Piece piece);
};

