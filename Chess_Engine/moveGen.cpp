#include "moveGen.h"

#include "utilities.h"
#include "boardMask.h"

MoveGen::MoveGen(Position& position) : pos(position) {}

void MoveGen::genMoves() {
	moves.clear();

	player = pos.turn == Color::WHITE ? white_mask() : black_mask();
	rival = pos.turn == Color::WHITE ? black_mask() : white_mask();
	occupied = player | rival;
	empty = ~occupied;

	genGuard();

	genPawn();
	genKnight();
	genSlidingPiece(pos.getBoard(pos.turn, ROOK), ROOK);
	genSlidingPiece(pos.getBoard(pos.turn, BISHOP), BISHOP);
	genSlidingPiece(pos.getBoard(pos.turn, QUEEN), QUEEN);
	genKing();

	guarded = 0ULL;
}

u64 MoveGen::white_mask() {
	return pos.white_pawns | pos.white_rooks | pos.white_knights | pos.white_bishops | pos.white_queens | pos.white_kings;
}

u64 MoveGen::black_mask() {
	return pos.black_pawns | pos.black_rooks | pos.black_knights | pos.black_bishops | pos.black_queens | pos.black_kings;
}

void MoveGen::genPawn() {
	u64 pawns = pos.getBoard(pos.turn, PAWN);

	if (pos.turn == Color::WHITE) {
		//Capture
		appendMoves(move<NORTH_EAST>(pawns) & rival & ~RANK_8, SOUTH_WEST, CAPTURE);
		appendMoves(move<NORTH_WEST>(pawns) & rival & ~RANK_8, SOUTH_EAST, CAPTURE);

		//Push
		appendMoves(move<NORTH>(pawns) & empty & ~RANK_8, SOUTH, QUIET);
		appendMoves(move<NORTH>(empty) & (move<NORTH2>(pawns & RANK_2) & empty), SOUTH2, DOUBLE_PAWN_PUSH);

		//Promotion
		appendPromotions(move<NORTH_EAST>(pawns) & rival & RANK_8, SOUTH_WEST, CAPTURE);
		appendPromotions(move<NORTH_WEST>(pawns) & rival & RANK_8, SOUTH_EAST, CAPTURE);
		appendPromotions(move<NORTH>(pawns) & empty & RANK_8, SOUTH, QUIET);

		//En-Passant
		if (pos.moveCount() > 0 && pos.lastMove().isDoublePawnPush()) {
			appendMoves(move<NORTH_EAST>(pawns & RANK_5) & BoardMask::enPassant(pos.lastMove()), SOUTH_WEST, EN_PASSANT);
			appendMoves(move<NORTH_WEST>(pawns & RANK_5) & BoardMask::enPassant(pos.lastMove()), SOUTH_EAST, EN_PASSANT);
		}
	}
	else {
		//Capture
		appendMoves(move<SOUTH_EAST>(pawns) & rival & ~RANK_1, NORTH_WEST, CAPTURE);
		appendMoves(move<SOUTH_WEST>(pawns) & rival & ~RANK_1, NORTH_EAST, CAPTURE);

		//Push
		appendMoves(move<SOUTH>(pawns) & empty & ~RANK_1, NORTH, QUIET);
		appendMoves(move<SOUTH>(empty) & (move<SOUTH2>(pawns & RANK_7) & empty), NORTH2, DOUBLE_PAWN_PUSH);

		//Promotion
		appendPromotions(move<SOUTH_EAST>(pawns) & rival & RANK_1, NORTH_WEST, CAPTURE);
		appendPromotions(move<SOUTH_WEST>(pawns) & rival & RANK_1, NORTH_EAST, CAPTURE);
		appendPromotions(move<SOUTH>(pawns) & empty & RANK_1, NORTH, QUIET);

		//En-Passant
		if (pos.moveCount() > 0 && pos.lastMove().isDoublePawnPush()) {
			appendMoves(move<SOUTH_EAST>(pawns & RANK_4) & BoardMask::enPassant(pos.lastMove()), NORTH_WEST, EN_PASSANT);
			appendMoves(move<SOUTH_WEST>(pawns & RANK_4) & BoardMask::enPassant(pos.lastMove()), NORTH_EAST, EN_PASSANT);
		}
	}
}

void MoveGen::genKnight() {
	u64 knights = pos.getBoard(pos.turn, KNIGHT);
	u64 knightPattern = 0x00000A1100110AULL;

	unsigned long idx;
	while (knights != 0) {
		_BitScanForward64(&idx, knights);

		int shiftMagnitude = idx - 18;
		u64 knightMove = shiftMagnitude > 0 ? knightPattern << shiftMagnitude : knightPattern >> std::abs(shiftMagnitude);
		if (((0x1ULL << idx) & KING_SIDE) != 0) knightMove &= ~(FILE_A | FILE_B);
		if (((0x1ULL << idx) & QUEEN_SIDE) != 0) knightMove &= ~(FILE_G | FILE_H);

		if ((pinned & BoardMask::board((Square)idx)) != 0) {
			knightMove &= pinDirections[(Square)idx];
		}

		if ((pos.getBoard(pos.turn, KING) & guarded) != 0) {
			knightMove &= checkResponses;
		}

		appendMoveMask((Square)idx, knightMove & empty, QUIET);
		appendMoveMask((Square)idx, knightMove & rival, CAPTURE);

		knights ^= 0x1ULL << idx;
	}
}

u64 MoveGen::slideLine(u64 piece, u64 line, u64 occ) {
	if (occ == NULL) occ = occupied;
	return (((occ & line) - (2 * piece)) ^ util::rev(util::rev(occ & line) - 2 * util::rev(piece))) & line;
}

u64 MoveGen::slidingAttack(Square square, Piece pieceType, u64 occ) {
	if (occ == NULL) occ = occupied;

	u64 attacks = 0ULL;
	u64 piece = BoardMask::board(square);

	if ((pieceType & ROOK) != 0) {
		attacks |= slideLine(piece, BoardMask::file(square), occ);
		attacks |= slideLine(piece, BoardMask::rank(square), occ);
	}
	if ((pieceType & BISHOP) != 0) {
		attacks |= slideLine(piece, BoardMask::diagonal(square), occ);
		attacks |= slideLine(piece, BoardMask::antiDiagonal(square), occ);
	}

	return attacks;
}

u64 MoveGen::xRayAttack(Square square, Piece pieceType, u64 blockers) {
	u64 attacks = slidingAttack(square, pieceType);
	blockers &= attacks;
	return attacks ^ slidingAttack(square, pieceType, occupied ^ blockers);
}

void MoveGen::genSlidingPiece(u64 pieces, Piece pieceType) {
	unsigned long idx;
	while (pieces != 0) {
		_BitScanForward64(&idx, pieces);

		Square source = (Square)idx;
		u64 attacks = slidingAttack(source, pieceType) & ~player;

		if ((pinned & BoardMask::board(source)) != 0) {
			attacks &= pinDirections[source];
		}

		if ((pos.getBoard(pos.turn, KING) & guarded) != 0) {
			attacks &= checkResponses;
		}

		appendMoveMask(source, attacks & empty, QUIET);
		appendMoveMask(source, attacks & rival, CAPTURE);

		pieces ^= 0x1ULL << idx;
	}
}

void MoveGen::genKing() {
	u64 kings = pos.getBoard(pos.turn, KING);
	u64 kingPattern = 0x00000000070507ULL;

	unsigned long idx;
	while (kings != 0) {
		_BitScanForward64(&idx, kings);
		int shiftMagnitude = idx - 9;
		u64 kingMove = shiftMagnitude > 0 ? kingPattern << shiftMagnitude : kingPattern >> std::abs(shiftMagnitude);
		if (((0x1ULL << idx) & KING_SIDE) != 0) kingMove &= ~(FILE_A);
		if (((0x1ULL << idx) & QUEEN_SIDE) != 0) kingMove &= ~(FILE_H);

		kingMove &= ~guarded;

		appendMoveMask((Square)idx, kingMove & empty, QUIET);
		appendMoveMask((Square)idx, kingMove & rival, CAPTURE);

		/*
		if (pos.turn == Color::WHITE) {
			if (pos.castling.white_kcastle = true && isSquareEmpty(S_F1) && isSquareEmpty(S_G1)) {
				moves.push_back(Move(Square(idx), S_G1, KING_SIDE_CASTLE));
			}
			if (pos.castling.white_qcastle = true && isSquareEmpty(S_D1) && isSquareEmpty(S_C1) && isSquareEmpty(S_B1)) {
				moves.push_back(Move(Square(idx), S_C1, QUEEN_SIDE_CASTLE));
			}
		}
		else {
			if (pos.castling.black_kcastle = true && isSquareEmpty(S_F8) && isSquareEmpty(S_G8)) {
				moves.push_back(Move(Square(idx), S_G8, KING_SIDE_CASTLE));
			}
			if (pos.castling.black_qcastle = true && isSquareEmpty(S_D8) && isSquareEmpty(S_C8) && isSquareEmpty(S_B8)) {
				moves.push_back(Move(Square(idx), S_C8, QUEEN_SIDE_CASTLE));
			}
		}
		*/

		kings ^= 0x1ULL << idx;
	}
}

void MoveGen::appendMoves(u64 source, Direction move, char moveFlags) {
	unsigned long idx;
	while (source != 0) {
		_BitScanForward64(&idx, source);

		if ((pos.getBoard(pos.turn, KING) & guarded) != 0) {
			if ((BoardMask::board((Square)idx) & checkResponses) == 0) {
				source ^= 1ULL << idx;
				continue;
			}
		}

		if ((BoardMask::board((Square)(idx + move)) & pinned) != 0) {
			if ((pinDirections[(Square)(idx + move)] & BoardMask::board((Square)idx)) == 0) {
				source ^= 1ULL << idx;
				continue;
			}
		}

		moves.push_back(Move(Square(idx + move), (Square)idx, moveFlags));
		source ^= 1ULL << idx;
	}
}

void MoveGen::appendPromotions(u64 source, Direction move, char moveFlags) {
	unsigned long idx;
	while (source != 0) {
		_BitScanForward64(&idx, source);

		if ((pos.getBoard(pos.turn, KING) & guarded) != 0) {
			if ((BoardMask::board((Square)idx) & checkResponses) == 0) {
				source ^= 1ULL << idx;
				continue;
			}
		}

		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | ROOK | moveFlags));
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | KNIGHT | moveFlags));
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | BISHOP | moveFlags));
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | QUEEN | moveFlags));
		source ^= 1ULL << idx;
	}
}

void MoveGen::appendMoveMask(Square piece, u64 mask, char moveFlags) {
	unsigned long idx;
	while (mask != 0) {
		_BitScanForward64(&idx, mask);

		/*
		if ((pos.getBoard(pos.turn, KING) & guarded) != 0) {
			if ((BoardMask::board((Square)idx) & checkResponses) == 0) {
				mask ^= 1ULL << idx;
				continue;
			}
		}
		*/

		moves.push_back(Move(piece, (Square)idx, moveFlags));
		mask ^= 1ULL << idx;
	}
}

void MoveGen::appendGuard(Square piece, u64 mask) {
	unsigned long idx;
	while (mask != 0) {
		_BitScanForward64(&idx, mask);
		guarded |= BoardMask::board((Square)idx);
		mask ^= 1ULL << idx;
	}
}

bool MoveGen::isSquareEmpty(Square square) {
	return ((0x1ULL << square) & occupied) == 0;
}

bool show = false;

long long MoveGen::perft(int depth, int initial) {
	if (depth == 0) {
		return 1;
	}
	else {
		genMoves();
		std::vector<Move> depthMoves(moves);

		long long count = 0;

		for (Move m : depthMoves) {
			pos.makeMove(m);

			long long partial = perft(depth - 1, initial);

			/*
			if (depth == initial && m.from() == S_F2 && m.to() == S_F3) {
				std::cout << initial << " ---------------- " << partial << " - ";
				m.print();
			}
			
			
			if (depth == initial - 1 && m.from() == S_G8 && m.to() == S_H6
				&& pos.history.at(0).from() == S_F2 && pos.history.at(0).to() == S_F3) {
				std::cout << initial - 1 << " ------------ " << partial << " - ";
				m.print();
			}
			
			
			if (depth == initial - 2 && m.from() == S_E1 && m.to() == S_F2 
				&& pos.history.at(1).from() == S_G8 && pos.history.at(1).to() == S_H6
				&& pos.history.at(0).from() == S_F2 && pos.history.at(0).to() == S_F3) {
				std::cout << initial - 2 << " -------- " << partial << " - ";
				m.print();
			}
			
			
			if (depth == initial - 3 // && m.from() == S_D8 && m.to() == S_A5
				&& pos.history.at(2).from() == S_E1 && pos.history.at(2).to() == S_F2
				&& pos.history.at(1).from() == S_G8 && pos.history.at(1).to() == S_H6
				&& pos.history.at(0).from() == S_F2 && pos.history.at(0).to() == S_F3) {
				std::cout << initial - 2 << " -------- " << partial << " - ";
				m.print();
			}

			
			if (depth == initial - 4  && m.from() == S_D8 && m.to() == S_A5
				&& pos.history.at(3).from() == S_D8 && pos.history.at(3).to() == S_A5
				&& pos.history.at(2).from() == S_D2 && pos.history.at(2).to() == S_D4
				&& pos.history.at(1).from() == S_C7 && pos.history.at(1).to() == S_C5
				&& pos.history.at(0).from() == S_B2 && pos.history.at(0).to() == S_B4) {
				std::cout << initial - 2 << " -------- " << partial << " - ";
				m.print();
			}
			*/

			count += partial;

			pos.unMakeMove();


		}
		return count;
	}
}

void MoveGen::genGuard() {
	guarded = 0ULL;

	Color enemyColor = pos.turn == Color::WHITE ? Color::BLACK : Color::WHITE;

	//PAWNS
	if (enemyColor == Color::WHITE) {
		guarded |= move<NORTH_EAST>(pos.getBoard(enemyColor, PAWN));
		guarded |= move<NORTH_WEST>(pos.getBoard(enemyColor, PAWN));
	}
	else {
		guarded |= move<SOUTH_EAST>(pos.getBoard(enemyColor, PAWN));
		guarded |= move<SOUTH_WEST>(pos.getBoard(enemyColor, PAWN));
	}

	//KNIGHTS
	u64 knights = pos.getBoard(enemyColor, KNIGHT);
	u64 knightPattern = 0x00000A1100110AULL;

	unsigned long idx;
	while (knights != 0) {
		_BitScanForward64(&idx, knights);
		int shiftMagnitude = idx - 18;
		u64 knightMove = shiftMagnitude > 0 ? knightPattern << shiftMagnitude : knightPattern >> std::abs(shiftMagnitude);
		if (((0x1ULL << idx) & KING_SIDE) != 0) knightMove &= ~(FILE_A | FILE_B);
		if (((0x1ULL << idx) & QUEEN_SIDE) != 0) knightMove &= ~(FILE_G | FILE_H);

		appendGuard((Square)idx, knightMove);

		knights ^= 0x1ULL << idx;
	}

	//ROOKS
	u64 rooks = pos.getBoard(enemyColor, ROOK);

	while (rooks != 0) {
		_BitScanForward64(&idx, rooks);
		appendGuard((Square)idx, slidingAttack((Square)idx, ROOK));
		appendGuard((Square)idx, xRayAttack((Square)idx, ROOK, pos.getBoard(pos.turn, KING)));
		rooks ^= 1ULL << idx;
	}

	//BISHOPS
	u64 bishops = pos.getBoard(enemyColor, BISHOP);

	while (bishops != 0) {
		_BitScanForward64(&idx, bishops);
		appendGuard((Square)idx, slidingAttack((Square)idx, BISHOP));
		appendGuard((Square)idx, xRayAttack((Square)idx, BISHOP, pos.getBoard(pos.turn, KING)));
		bishops ^= 1ULL << idx;
	}

	//QUEENS
	u64 queens = pos.getBoard(enemyColor, QUEEN);

	while (queens != 0) {
		_BitScanForward64(&idx, queens);
		appendGuard((Square)idx, slidingAttack((Square)idx, QUEEN));
		appendGuard((Square)idx, xRayAttack((Square)idx, QUEEN, pos.getBoard(pos.turn, KING)));
		queens ^= 1ULL << idx;
	}

	//KINGS
	u64 kings = pos.getBoard(enemyColor, KING);
	u64 kingPattern = 0x00000000070507ULL;

	while (kings != 0) {
		_BitScanForward64(&idx, kings);
		int shiftMagnitude = idx - 9;
		u64 kingMove = shiftMagnitude > 0 ? kingPattern << shiftMagnitude : kingPattern >> std::abs(shiftMagnitude);
		if (((0x1ULL << idx) & KING_SIDE) != 0) kingMove &= ~(FILE_A);
		if (((0x1ULL << idx) & QUEEN_SIDE) != 0) kingMove &= ~(FILE_H);
		appendGuard((Square)idx, kingMove);

		kings ^= 0x1ULL << idx;
	}

	//PINS
	pinned = 0;
	Square selfKing = BoardMask::square(pos.getBoard(pos.turn, KING));

	u64 pinners = pos.getBoard(enemyColor, ROOK) | pos.getBoard(enemyColor, QUEEN);
	pinners &= xRayAttack(selfKing, ROOK, player);
	while (pinners != 0) {
		_BitScanForward64(&idx, pinners);

		u64 pin = slidingAttack((Square)idx, ROOK) & player;
		Square pinnedSquare = BoardMask::square(pin);
		u64 pinDirectionKing = slidingAttack(pinnedSquare, ROOK) & slidingAttack(selfKing, ROOK);
		u64 pinDirectionPinner = slidingAttack(pinnedSquare, ROOK) & slidingAttack((Square)idx, ROOK);
		u64 partialPin = pinDirectionKing | pinDirectionPinner | BoardMask::board((Square)idx);
		pinDirections[pinnedSquare] = partialPin;

		pinned |= pin;
		pinners ^= 0x1ULL << idx;
	}

	pinners = pos.getBoard(enemyColor, BISHOP) | pos.getBoard(enemyColor, QUEEN);
	pinners &= xRayAttack(selfKing, BISHOP, player);
	while (pinners != 0) {
		_BitScanForward64(&idx, pinners);

		u64 pin = slidingAttack((Square)idx, BISHOP) & player;
		Square pinnedSquare = BoardMask::square(pin);
		u64 pinDirectionKing = slidingAttack(pinnedSquare, BISHOP) & slidingAttack(selfKing, BISHOP);
		u64 pinDirectionPinner = slidingAttack(pinnedSquare, BISHOP) & slidingAttack((Square)idx, BISHOP);
		u64 partialPin = pinDirectionKing | pinDirectionPinner | BoardMask::board((Square)idx);
		pinDirections[pinnedSquare] = partialPin;

		pinned |= pin;
		pinners ^= 0x1ULL << idx;
	}

	pinned &= slidingAttack(selfKing, QUEEN);

	//CHECK COVER
	checkResponses = 0;

	u64 rookCheckers = pos.getBoard(enemyColor, ROOK) | pos.getBoard(enemyColor, QUEEN);
	u64 rookKing = slidingAttack(selfKing, ROOK);
	rookCheckers &= rookKing;
	while (rookCheckers != 0) {
		_BitScanForward64(&idx, rookCheckers);
		Square checkingSquare = (Square)idx;

		u64 checkCover = rookKing & slidingAttack(checkingSquare, ROOK);
		checkCover |= BoardMask::board(checkingSquare);
		checkResponses |= checkCover;

		rookCheckers ^= 0x1ULL << idx;
	}

	u64 bishopCheckers = pos.getBoard(enemyColor, BISHOP) | pos.getBoard(enemyColor, QUEEN);
	u64 bishopKing = slidingAttack(selfKing, BISHOP);
	bishopCheckers &= bishopKing;
	while (bishopCheckers != 0) {
		_BitScanForward64(&idx, bishopCheckers);
		Square checkingSquare = (Square)idx;

		u64 checkCover = bishopKing & slidingAttack(checkingSquare, BISHOP);
		checkCover |= BoardMask::board(checkingSquare);
		checkResponses |= checkCover;

		bishopCheckers ^= 0x1ULL << idx;
	}

	knights = pos.getBoard(enemyColor, KNIGHT);
	knightPattern = 0x00000A1100110AULL;

	while (knights != 0) {
		_BitScanForward64(&idx, knights);
		int shiftMagnitude = idx - 18;
		u64 knightMove = shiftMagnitude > 0 ? knightPattern << shiftMagnitude : knightPattern >> std::abs(shiftMagnitude);
		if (((0x1ULL << idx) & KING_SIDE) != 0) knightMove &= ~(FILE_A | FILE_B);
		if (((0x1ULL << idx) & QUEEN_SIDE) != 0) knightMove &= ~(FILE_G | FILE_H);

		if ((knightMove & pos.getBoard(pos.turn, KING)) != 0) {
			checkResponses |= BoardMask::board((Square)idx);
		}

		knights ^= 0x1ULL << idx;
	}
}