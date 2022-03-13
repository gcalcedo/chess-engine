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

u64 MoveGen::white_mask() {
	return pos.white_pawns | pos.white_rooks | pos.white_knights | pos.white_bishops | pos.white_queens | pos.white_kings;
}

u64 MoveGen::black_mask() {
	return pos.black_pawns | pos.black_rooks | pos.black_knights | pos.black_bishops | pos.black_queens | pos.black_kings;
}

void MoveGen::genPawn() {
	u64 pawns = pos.getBoard(turn, PAWN);

	appendMoves(move<NORTH_EAST>(pawns) & rival & ~RANK_8, SOUTH_WEST);
	appendMoves(move<NORTH_WEST>(pawns) & rival & ~RANK_8, SOUTH_EAST);

	appendMoves(move<NORTH>(pawns) & empty & ~RANK_8, SOUTH);
	appendMoves(move<NORTH>(empty) & (move<NORTH2>(pawns & RANK_2) & empty), SOUTH2);

	appendPromotions(move<NORTH_EAST>(pawns) & rival & RANK_8, SOUTH_WEST);
	appendPromotions(move<NORTH_WEST>(pawns) & rival & RANK_8, SOUTH_EAST);
	appendPromotions(move<NORTH>(pawns) & empty & RANK_8, SOUTH);

	for (Move move : moves) {
		move.print();
		if (move.isQuiet()) {
			std::cout << move.getFrom() << " -> " << move.getTo() << std::endl;
		}
		if (move.isCapture()) {
			std::cout << move.getFrom() << " -> " << move.getTo() << " Capture!" << std::endl;
		}
		if (move.isPromotion()) {
			std::cout << move.getFrom() << " -> " << move.getTo() << " | " << move.getPromotion() << std::endl;
		}
	}
}

void MoveGen::appendMoves(u64 source, Direction move) {
	unsigned long idx;
	while (source != 0) {
		_BitScanForward64(&idx, source);
		char isCapture = (rival & (1ULL << idx)) != 0 ? CAPTURE : QUIET;
		moves.push_back(Move(Square(idx + move), (Square)idx, isCapture));
		source ^= 1ULL << idx;
	}
}

void MoveGen::appendPromotions(u64 source, Direction move) {
	unsigned long idx;
	while (source != 0) {
		_BitScanForward64(&idx, source);
		char isCapture = (rival & (1ULL << idx)) != 0 ? CAPTURE : QUIET;
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | ROOK | isCapture));
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | KNIGHT | isCapture));
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | BISHOP | isCapture));
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | QUEEN | isCapture));
		source ^= 1ULL << idx;
	}
}