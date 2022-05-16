#include "evaluation.h"

Evaluation::Evaluation(Position& position, MoveGen& moveGen) : pos(position), gen(moveGen) {};

int Evaluation::pieceValue(Piece piece) {
	if (piece == KNIGHT) return KNIGHT_VALUE;
	if (piece == BISHOP) return BISHOP_VALUE;
	if (piece == ROOK) return ROOK_VALUE;
	if (piece == QUEEN) return QUEEN_VALUE;
	if (piece == PAWN) return PAWN_VALUE;
}

std::vector<Move> Evaluation::sortMoves(std::vector<Move> moves, Move hashMove) {
	for (auto& move : moves) {
		if (move.from() == hashMove.from() && move.to() == hashMove.to()) {
			move.score += 10000;
		}

		Piece self = pos.getPiece(move.from());

		if (move.isCapture()) {
			Piece capture = pos.getPiece(move.to());
			move.score += 10 * pieceValue(capture) - pieceValue(capture);

			if ((BoardMask::board(move.to()) & gen.guarded) != 0) {
				move.score -= pieceValue(capture);
			}
		}

		if (move.isPromotion()) {
			move.score += pieceValue(move.getPromotion());
		}
	}

	std::sort(moves.begin(), moves.end(),
		[](const Move& a, const Move& b) -> bool
		{
			return a.score > b.score;
		});

	return moves;
}

int Evaluation::evaluate() {
	int evaluation = 0;

	int white_material = materialCount(Color::WHITE);
	int black_material = materialCount(Color::BLACK);

	if (gen.genMoves().size() == 0) {
		if (gen.checkersCount > 0) {
			evaluation += pos.turnColor == BLACK ? 10000 : -10000;
		}
		else {
			return 0;
		}
	}

	evaluation += white_material - black_material;
	return evaluation;
}

int Evaluation::materialCount(Color color) {
	int pawns = __popcnt64(pos.getBoard(color, PAWN)) * PAWN_VALUE;
	int knights = __popcnt64(pos.getBoard(color, KNIGHT)) * KNIGHT_VALUE;
	int bishops = __popcnt64(pos.getBoard(color, BISHOP)) * BISHOP_VALUE;
	int rooks = __popcnt64(pos.getBoard(color, ROOK)) * ROOK_VALUE;
	int queens = __popcnt64(pos.getBoard(color, QUEEN)) * QUEEN_VALUE;

	return pawns + knights + bishops + rooks + queens;
}
