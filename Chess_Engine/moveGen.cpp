#include "moveGen.h"

MoveGen::MoveGen(Position& position) : pos(position) {}

void MoveGen::genMoves(Color color) {
	turn = color;

	player = turn == Color::WHITE ? white_mask() : black_mask();
	rival = turn == Color::WHITE ? black_mask() : white_mask();
	occupied = player | rival;
	empty = ~occupied;

	genPawn();
}

U64 MoveGen::white_mask() {
	return pos.white_pawns | pos.white_rooks | pos.white_knights | pos.white_bishops | pos.white_queens | pos.white_kings;
}

U64 MoveGen::black_mask() {
	return pos.black_pawns | pos.black_rooks | pos.black_knights | pos.black_bishops | pos.black_queens | pos.black_kings;
}

void MoveGen::genPawn() {
	U64 moves = 0LL;

	pos.printBitBoard(pos.white_pawns);

	U64 east_captures = move<NORTH_EAST>(pos.white_pawns) & rival;
	U64 west_captures = move<NORTH_WEST>(pos.white_pawns) & rival;
	U64 forward = move<NORTH>(pos.white_pawns) & empty;
	U64 forward_double = move<NORTH>(forward) & (move<NORTH2>(pos.white_pawns & RANK_2) & empty);

	pos.printBitBoard(forward_double);
}