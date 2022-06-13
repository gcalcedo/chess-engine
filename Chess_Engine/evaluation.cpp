#include "evaluation.h"


std::vector<int> flip_vertical(const int nrows, const int ncols, std::vector<int> table)  // flips: bottom-up
{
	std::vector<int> data(table);

	for (int cc = 0; cc < ncols; cc++)
	{
		for (int rr = 0; rr < nrows / 2; rr++)
		{
			int rrInv = nrows - 1 - rr;
			std::swap(data[rr * ncols + cc], data[rrInv * ncols + cc]);
		}
	}

	return data;
}

Evaluation::Evaluation(Position& position, MoveGen& moveGen) : pos(position), gen(moveGen) {
	std::vector<int> pawn = {
		  0,  0,  0,  0,  0,  0,  0,  0,
		 50, 50, 50, 50, 50, 50, 50, 50,
		 10, 10, 20, 30, 30, 20, 10, 10,
		  5,  5, 10, 25, 25, 10,  5,  5,
		  0,  0,  0, 20, 20,  0,  0,  0,
		  5, -5,-10,  0,  0,-10, -5,  5,
		  5, 10, 10,-20,-20, 10, 10,  5,
		  0,  0,  0,  0,  0,  0,  0,  0 
	};
	std::vector<int> knight = {
		-50,-40,-30,-30,-30,-30,-40,-50,
		-40,-20,  0,  0,  0,  0,-20,-40,
		-30,  0, 10, 15, 15, 10,  0,-30,
		-30,  5, 15, 20, 20, 15,  5,-30,
		-30,  0, 15, 20, 20, 15,  0,-30,
		-30,  5, 10, 15, 15, 10,  5,-30,
		-40,-20,  0,  5,  5,  0,-20,-40,
		-50,-40,-30,-30,-30,-30,-40,-50,
	};
	std::vector<int> bishop = {
		-20,-10,-10,-10,-10,-10,-10,-20,
		-10,  0,  0,  0,  0,  0,  0,-10,
		-10,  0,  5, 10, 10,  5,  0,-10,
		-10,  5,  5, 10, 10,  5,  5,-10,
		-10,  0, 10, 10, 10, 10,  0,-10,
		-10, 10, 10, 10, 10, 10, 10,-10,
		-10,  5,  0,  0,  0,  0,  5,-10,
		-20,-10,-10,-10,-10,-10,-10,-20,
	};
	std::vector<int> rook = {
		  0,  0,  0,  0,  0,  0,  0,  0,
		  5, 10, 10, 10, 10, 10, 10,  5,
		 -5,  0,  0,  0,  0,  0,  0, -5,
		 -5,  0,  0,  0,  0,  0,  0, -5,
		 -5,  0,  0,  0,  0,  0,  0, -5,
		 -5,  0,  0,  0,  0,  0,  0, -5,
		 -5,  0,  0,  0,  0,  0,  0, -5,
		  0,  0,  0, 10, 10,  5,  0,  0
	};
	std::vector<int> queen = {
		-20,-10,-10, -5, -5,-10,-10,-20,
		-10,  0,  0,  0,  0,  0,  0,-10,
		-10,  0,  5,  5,  5,  5,  0,-10,
		 -5,  0,  5,  5,  5,  5,  0, -5,
		  0,  0,  5,  5,  5,  5,  0, -5,
		-10,  5,  5,  5,  5,  5,  0,-10,
		-10,  0,  5,  0,  0,  0,  0,-10,
		-20,-10,-10, -5, -5,-10,-10,-20
	};
	std::vector<int> king = {
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-20,-30,-30,-40,-40,-30,-30,-20,
		-10,-20,-20,-20,-20,-20,-20,-10,
		 20, 20,  0,  0,  0,  0, 20, 20,
		 20, 50, 10,  0,  0, 10, 50, 20
	};

	std::unordered_map<Piece, std::vector<int>> white_pieces;
	white_pieces.emplace(PAWN, pawn);
	white_pieces.emplace(KNIGHT, knight);
	white_pieces.emplace(BISHOP, bishop);
	white_pieces.emplace(ROOK, rook);
	white_pieces.emplace(QUEEN, queen);
	white_pieces.emplace(KING, king);

	std::unordered_map<Piece, std::vector<int>> black_pieces;
	black_pieces.emplace(PAWN, flip_vertical(8, 8, pawn));
	black_pieces.emplace(KNIGHT, flip_vertical(8, 8, knight));
	black_pieces.emplace(BISHOP, flip_vertical(8, 8, bishop));
	black_pieces.emplace(ROOK, flip_vertical(8, 8, rook));
	black_pieces.emplace(QUEEN, flip_vertical(8, 8, queen));
	black_pieces.emplace(KING, flip_vertical(8, 8, king));

	pieceSquareTable.emplace(WHITE, white_pieces);
	pieceSquareTable.emplace(BLACK, black_pieces);
};

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
	evaluation += white_material - black_material;

	evaluation += pieceSquareEval(pos.turnColor);

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

int Evaluation::pieceSquareEval(Color color) {
	int pieceSquareEval = 0;

	for (Piece piece : PieceList) {
		u64 board = pos.getBoard(color, piece);
		unsigned long idx = 0;
		while (board != 0) {
			_BitScanForward64(&idx, board);
			pieceSquareEval += pieceSquareTable[color][piece][(Square)idx] * color;
			board ^= 0x1ULL << idx;
		}
	}

	return pieceSquareEval;
}
