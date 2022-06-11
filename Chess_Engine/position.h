#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "types.h"
#include "transposition.h"
#include <map>
#include <unordered_map>
#include <boost/unordered_map.hpp>

class Transposition;

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

	Transposition trans = Transposition();
	u64 zobristKey = 0ULL;

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

	u64* boards[2][6] = {
		{
			&white_knights,
			&white_bishops,
			&white_rooks,
			&white_queens,
			&white_kings,
			&white_pawns
		},
		{
			&black_knights,
			&black_bishops,
			&black_rooks,
			&black_queens,
			&black_kings,
			&black_pawns
		}
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
	//char board[8][8] =
	//{
	//	{'r', '-', 'b', 'q', 'k', 'b', 'n', 'r'},
	//	{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
	//	{'-', '-', '-', '-', '-', '-', '-', '-'},
	//	{'-', '-', '-', '-', '-', '-', '-', '-'},
	//	{'-', 'n', '-', '-', '-', '-', '-', '-'},
	//	{'-', '-', 'N', '-', 'P', '-', '-', '-'},
	//	{'P', 'P', 'P', 'P', '-', 'P', 'P', 'P'},
	//	{'R', '-', 'B', 'Q', 'K', 'B', 'N', 'R'}
	//};
	//char board[8][8] =
	//{
	//	{'r', 'n', 'b', 'q', 'k', 'b', '-', 'r'},
	//	{'-', 'p', '-', '-', 'p', 'p', 'p', 'p'},
	//	{'p', '-', '-', 'p', '-', 'n', '-', '-'},
	//	{'-', '-', '-', '-', '-', '-', '-', '-'},
	//	{'-', '-', '-', 'N', 'P', '-', '-', '-'},
	//	{'-', '-', 'N', '-', '-', '-', '-', '-'},
	//	{'P', 'P', 'P', '-', '-', 'P', 'P', 'P'},
	//	{'R', '-', 'B', 'Q', 'K', 'B', '-', 'R'}
	//};
	//char board[8][8] =
	//{
	//	{'r', '-', 'b', 'q', 'k', 'b', 'n', 'r'},
	//	{'-', 'p', 'p', 'p', '-', 'p', 'p', 'p'},
	//	{'p', '-', 'n', '-', '-', '-', '-', '-'},
	//	{'-', 'B', '-', '-', 'p', '-', '-', '-'},
	//	{'-', '-', '-', '-', 'P', '-', '-', '-'},
	//	{'-', '-', '-', '-', '-', 'N', '-', '-'},
	//	{'P', 'P', 'P', 'P', '-', 'P', 'P', 'P'},
	//	{'R', 'N', 'B', 'Q', 'K', '-', '-', 'R'}
	//};
	//char board[8][8] =
	//{
	//	{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
	//	{'p', 'p', 'p', '-', '-', 'p', 'p', 'p'},
	//	{'-', '-', '-', '-', 'p', '-', '-', '-'},
	//	{'-', '-', '-', 'p', '-', '-', '-', '-'},
	//	{'-', '-', 'P', 'P', '-', '-', '-', '-'},
	//	{'-', '-', '-', '-', '-', '-', '-', '-'},
	//	{'P', 'P', '-', '-', 'P', 'P', 'P', 'P'},
	//	{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
	//};
	//char board[8][8] =
	//{
	//	{'-', '-', '-', '-', '-', '-', '-', '-'},
	//	{'k', '-', '-', '-', '-', '-', '-', '-'},
	//	{'-', '-', '-', 'p', '-', '-', '-', '-'},
	//	{'p', '-', '-', 'P', '-', 'p', '-', '-'},
	//	{'P', '-', '-', 'P', '-', 'P', '-', '-'},
	//	{'-', '-', '-', '-', '-', '-', '-', '-'},
	//	{'-', '-', '-', '-', '-', '-', '-', '-'},
	//	{'K', '-', '-', '-', '-', '-', '-', '-'}
	//};
	//char board[8][8] =
	//{
	//	{'-', 'r', '-', '-', '-', '-', 'k', '-'},
	//	{'p', '-', '-', 'n', '-', 'p', 'p', '-'},
	//	{'-', '-', '-', 'q', 'p', '-', '-', 'p'},
	//	{'-', '-', '-', '-', '-', '-', '-', '-'},
	//	{'-', '-', 'P', 'B', '-', '-', '-', '-'},
	//	{'-', '-', 'P', 'B', '-', '-', '-', '-'},
	//	{'-', '-', 'Q', '-', '-', 'K', '-', 'P'},
	//	{'R', '-', '-', '-', '-', '-', '-', '-'}
	//};


public:
	Position(std::string fen);

	void init();
	Piece getPiece(Square square);
	u64 getBoard(Color color, Piece piece);

	void makeMove(Move move);
	void unMakeMove();
	int moveCount();
	Move lastMove();

	u64 zobristHash();
	void print();

private:
	u64* boardReference(Square square);
	u64* boardReference(Color color, Piece piece);

};

