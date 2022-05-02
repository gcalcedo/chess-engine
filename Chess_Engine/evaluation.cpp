#include "evaluation.h"

Evaluation::Evaluation(Position& position) : pos(position) {};

int Evaluation::evaluate() {
	int white_material = materialCount(Color::WHITE);
	int black_material = materialCount(Color::BLACK);

	return white_material - black_material;
}

int Evaluation::materialCount(Color color) {
	int pawns = __popcnt64(pos.getBoard(color, PAWN)) * PAWN_VALUE;
	int knights = __popcnt64(pos.getBoard(color, KNIGHT)) * KNIGHT_VALUE;
	int bishops = __popcnt64(pos.getBoard(color, BISHOP)) * BISHOP_VALUE;
	int rooks = __popcnt64(pos.getBoard(color, ROOK)) * ROOK_VALUE;
	int queens = __popcnt64(pos.getBoard(color, QUEEN)) * QUEEN_VALUE;

	return pawns + knights + bishops + rooks + queens;
}
