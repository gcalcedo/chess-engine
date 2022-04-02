#include "moveGen.h"

#include "utilities.h"
#include "boardMask.h"

MoveGen::MoveGen(Position& position) : pos(position) {}

void MoveGen::genMoves() {
	moves.clear();

	player = pos.turnColor == Color::WHITE ? white_mask() : black_mask();
	rival = pos.turnColor == Color::WHITE ? black_mask() : white_mask();
	occupied = player | rival;
	empty = ~occupied;

	genGuard();

	genPawn();
	genKnight();
	genSlidingPiece(pos.getBoard(pos.turnColor, ROOK), ROOK);
	genSlidingPiece(pos.getBoard(pos.turnColor, BISHOP), BISHOP);
	genSlidingPiece(pos.getBoard(pos.turnColor, QUEEN), QUEEN);
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
	if (pos.moveCount() > 0 && pos.lastMove().isDoublePawnPush() && (BoardMask::board(pos.lastMove().to()) & checkers) != 0) {
		checkResponses |= BoardMask::board((Square)(pos.lastMove().to() + (pos.turnColor == Color::WHITE ? 8 : -8)));
	}

	u64 pawns = pos.getBoard(pos.turnColor, PAWN);

	if (pos.turnColor == Color::WHITE) {
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
			appendMoves(move<NORTH_EAST>(pawns & RANK_5) & BoardMask::enPassant(pos.lastMove(), pos.turnColor), SOUTH_WEST, EN_PASSANT);
			appendMoves(move<NORTH_WEST>(pawns & RANK_5) & BoardMask::enPassant(pos.lastMove(), pos.turnColor), SOUTH_EAST, EN_PASSANT);
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
			appendMoves(move<SOUTH_EAST>(pawns & RANK_4) & BoardMask::enPassant(pos.lastMove(), pos.turnColor), NORTH_WEST, EN_PASSANT);
			appendMoves(move<SOUTH_WEST>(pawns & RANK_4) & BoardMask::enPassant(pos.lastMove(), pos.turnColor), NORTH_EAST, EN_PASSANT);
		}
	}

	if (pos.moveCount() > 0 &&  pos.lastMove().isDoublePawnPush() && (BoardMask::board(pos.lastMove().to()) & checkers) != 0) {
		checkResponses &= ~BoardMask::board((Square)(pos.lastMove().to() + (pos.turnColor == Color::WHITE ? 8 : -8)));
	}
}

void MoveGen::genKnight() {
	u64 knights = pos.getBoard(pos.turnColor, KNIGHT);
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

		if ((pos.getBoard(pos.turnColor, KING) & guarded) != 0) {
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

		if ((pos.getBoard(pos.turnColor, KING) & guarded) != 0) {
			attacks &= checkResponses;
		}

		appendMoveMask(source, attacks & empty, QUIET);
		appendMoveMask(source, attacks & rival, CAPTURE);

		pieces ^= 0x1ULL << idx;
	}
}

void MoveGen::genKing() {
	u64 kings = pos.getBoard(pos.turnColor, KING);
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

		if ((pos.getBoard(pos.turnColor, KING) & guarded) == 0) {
			if (pos.turnColor == Color::WHITE) {
				if (pos.castling.white_kcastle == true && isSquareSafe(S_F1) && isSquareSafe(S_G1)) {
					moves.push_back(Move(Square(idx), S_G1, KING_SIDE_CASTLE));
				}
				if (pos.castling.white_qcastle == true && isSquareSafe(S_D1) && isSquareSafe(S_C1) & isSquareEmpty(S_B1)) {
					moves.push_back(Move(Square(idx), S_C1, QUEEN_SIDE_CASTLE));
				}
			}
			else {
				if (pos.castling.black_kcastle == true && isSquareSafe(S_F8) && isSquareSafe(S_G8)) {
					moves.push_back(Move(Square(idx), S_G8, KING_SIDE_CASTLE));
				}
				if (pos.castling.black_qcastle == true && isSquareSafe(S_D8) && isSquareSafe(S_C8) & isSquareEmpty(S_B8)) {
					moves.push_back(Move(Square(idx), S_C8, QUEEN_SIDE_CASTLE));
				}
			}
		}

		kings ^= 0x1ULL << idx;
	}
}

void MoveGen::appendMoves(u64 source, Direction move, char moveFlags) {
	unsigned long idx;
	while (source != 0) {
		_BitScanForward64(&idx, source);

		if ((pos.getBoard(pos.turnColor, KING) & guarded) != 0) {
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

		if (moveFlags == EN_PASSANT) {
			u64 selfKing = pos.getBoard(pos.turnColor, KING);
			u64 rq = pos.getBoard(pos.turnColor == Color::WHITE ? Color::BLACK : Color::WHITE, ROOK);
			rq |= pos.getBoard(pos.turnColor == Color::WHITE ? Color::BLACK : Color::WHITE, QUEEN);
			Square pawnSquare = (Square)(idx + move);
			u64 pawnProjection = slideLine(
				BoardMask::board(pawnSquare),
				BoardMask::rank(pawnSquare),
				occupied & ~BoardMask::board(pos.lastMove().to()));
			if ((pawnProjection & selfKing) != 0 && (pawnProjection & rq) != 0) {
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

		if ((pos.getBoard(pos.turnColor, KING) & guarded) != 0) {
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

bool MoveGen::isSquareSafe(Square square) {
	return (BoardMask::board(square) & occupied) == 0 && (BoardMask::board(square) & guarded) == 0;
}

bool MoveGen::isSquareEmpty(Square square) {
	return (BoardMask::board(square) & occupied) == 0;
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
		

			//if (depth == initial) {
			//	std::cout << partial << " - ";
			//	m.print();
			//}

			count += partial;

			pos.unMakeMove();
		}
		return count;
	}
}

void MoveGen::genGuard() {
	guarded = 0ULL;

	Color enemyColor = pos.turnColor == Color::WHITE ? Color::BLACK : Color::WHITE;

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
		appendGuard((Square)idx, xRayAttack((Square)idx, ROOK, pos.getBoard(pos.turnColor, KING)));
		rooks ^= 1ULL << idx;
	}

	//BISHOPS
	u64 bishops = pos.getBoard(enemyColor, BISHOP);

	while (bishops != 0) {
		_BitScanForward64(&idx, bishops);
		appendGuard((Square)idx, slidingAttack((Square)idx, BISHOP));
		appendGuard((Square)idx, xRayAttack((Square)idx, BISHOP, pos.getBoard(pos.turnColor, KING)));
		bishops ^= 1ULL << idx;
	}

	//QUEENS
	u64 queens = pos.getBoard(enemyColor, QUEEN);

	while (queens != 0) {
		_BitScanForward64(&idx, queens);
		appendGuard((Square)idx, slidingAttack((Square)idx, QUEEN));
		appendGuard((Square)idx, xRayAttack((Square)idx, QUEEN, pos.getBoard(pos.turnColor, KING)));
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
	Square selfKing = BoardMask::square(pos.getBoard(pos.turnColor, KING));

	u64 pinners = pos.getBoard(enemyColor, ROOK) | pos.getBoard(enemyColor, QUEEN);
	pinners &= xRayAttack(selfKing, ROOK, player);
	while (pinners != 0) {
		_BitScanForward64(&idx, pinners);

		u64 pin = slidingAttack((Square)idx, ROOK) & player & slidingAttack(selfKing, ROOK);
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

		u64 pin = slidingAttack((Square)idx, BISHOP) & player & slidingAttack(selfKing, BISHOP);
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
	checkResponses = 0ULL;
	checkers = 0ULL;
	checkersCount = 0;

	u64 rookCheckers = pos.getBoard(enemyColor, ROOK) | pos.getBoard(enemyColor, QUEEN);
	u64 rookKing = slidingAttack(selfKing, ROOK);
	rookCheckers &= rookKing;
	checkers |= rookCheckers;
	while (rookCheckers != 0) {
		checkersCount++;
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
	checkers |= bishopCheckers;
	while (bishopCheckers != 0) {
		checkersCount++;
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

		if ((knightMove & pos.getBoard(pos.turnColor, KING)) != 0) {
			checkResponses |= BoardMask::board((Square)idx);
			checkers |= BoardMask::board((Square)idx);
			checkersCount++;
		}

		knights ^= 0x1ULL << idx;
	}

	if (enemyColor == Color::WHITE) {
		u64 checks = move<NORTH_EAST>(pos.getBoard(enemyColor, PAWN)) & pos.getBoard(pos.turnColor, KING);
		if (checks != 0) checkersCount++;
		checkResponses |= move<SOUTH_WEST>(checks);
		checkers |= move<SOUTH_WEST>(checks);
		
		checks = move<NORTH_WEST>(pos.getBoard(enemyColor, PAWN)) & pos.getBoard(pos.turnColor, KING);
		if (checks != 0) checkersCount++;
		checkResponses |= move<SOUTH_EAST>(checks);
		checkers |= move<SOUTH_EAST>(checks);
	}
	else {
		u64 checks = move<SOUTH_EAST>(pos.getBoard(enemyColor, PAWN)) & pos.getBoard(pos.turnColor, KING);
		if (checks != 0) checkersCount++;
		checkResponses |= move<NORTH_WEST>(checks);
		checkers |= move<NORTH_WEST>(checks);

		checks = move<SOUTH_WEST>(pos.getBoard(enemyColor, PAWN)) & pos.getBoard(pos.turnColor, KING);
		if (checks != 0) checkersCount++;
		checkResponses |= move<NORTH_EAST>(checks);
		checkers |= move<NORTH_EAST>(checks);
	}

	if (checkersCount >= 2) checkResponses = 0;
}