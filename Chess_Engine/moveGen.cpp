#include "moveGen.h"

MoveGen::MoveGen(Position& position) : pos(position) {}

void MoveGen::genMoves(Color color) {
	moves.clear();

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
	U64 east_captures = move<NORTH_EAST>(pos.white_pawns) & rival;
	appendMoves(&moves, east_captures, SOUTH_WEST);

	U64 west_captures = move<NORTH_WEST>(pos.white_pawns) & rival;
	appendMoves(&moves, west_captures, SOUTH_EAST);

	U64 forward = move<NORTH>(pos.white_pawns) & empty;
	appendMoves(&moves, forward, SOUTH);

	U64 forward_double = move<NORTH>(forward) & (move<NORTH2>(pos.white_pawns & RANK_2) & empty);
	appendMoves(&moves, forward_double, SOUTH2);

	for (Move m : moves) {
		std::cout << m.from << " -> " << m.to << std::endl;
	}
}

void MoveGen::appendMoves(std::vector<Move> *target, U64 source, Direction move) {
	unsigned long idx;
	while (source != 0) {
		_BitScanForward64(&idx, source);
		target->push_back(Move((Square) (idx + move), (Square) idx));
		source ^= 1ULL << idx;
	}
}