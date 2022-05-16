#include "position.h"
#include "boardMask.h"

Position::Position(std::string fen) {
	init();
}

void Position::init() {
	std::string binary;

	for (size_t x = 0; x < 8; ++x) {
		for (size_t y = 0; y < 8; ++y) {
			binary = "0000000000000000000000000000000000000000000000000000000000000000";
			binary = binary.substr(0, x * 8 + y) + "1" + binary.substr(x * 8 + y + 1);

			switch (board[x][y]) {
			case 'P': white_pawns += std::stoull(binary, nullptr, 2); break;
			case 'R': white_rooks += std::stoull(binary, nullptr, 2); break;
			case 'N': white_knights += std::stoull(binary, nullptr, 2); break;
			case 'B': white_bishops += std::stoull(binary, nullptr, 2); break;
			case 'Q': white_queens += std::stoull(binary, nullptr, 2); break;
			case 'K': white_kings += std::stoull(binary, nullptr, 2); break;
			case 'p': black_pawns += std::stoull(binary, nullptr, 2); break;
			case 'r': black_rooks += std::stoull(binary, nullptr, 2); break;
			case 'n': black_knights += std::stoull(binary, nullptr, 2); break;
			case 'b': black_bishops += std::stoull(binary, nullptr, 2); break;
			case 'q': black_queens += std::stoull(binary, nullptr, 2); break;
			case 'k': black_kings += std::stoull(binary, nullptr, 2); break;
			}
		}
	}

	zobristKey = zobristHash();
}

Piece Position::getPiece(Square square) {
	u64 boardMask = BoardMask::board(square);
	if ((white_pawns & boardMask) != 0) return PAWN;
	if ((white_rooks & boardMask) != 0) return ROOK;
	if ((white_knights & boardMask) != 0) return KNIGHT;
	if ((white_bishops & boardMask) != 0) return BISHOP;
	if ((white_queens & boardMask) != 0) return QUEEN;
	if ((white_kings & boardMask) != 0) return KING;
	if ((black_pawns & boardMask) != 0) return PAWN;
	if ((black_rooks & boardMask) != 0) return ROOK;
	if ((black_knights & boardMask) != 0) return KNIGHT;
	if ((black_bishops & boardMask) != 0) return BISHOP;
	if ((black_queens & boardMask) != 0) return QUEEN;
	if ((black_kings & boardMask) != 0) return KING;
	return (Piece) -1;
}

u64 Position::getBoard(Color color, Piece piece) {
	switch (piece) {
	case PAWN: return color == Color::WHITE ? white_pawns : black_pawns;
	case ROOK: return color == Color::WHITE ? white_rooks : black_rooks;
	case KNIGHT: return color == Color::WHITE ? white_knights : black_knights;
	case BISHOP: return color == Color::WHITE ? white_bishops : black_bishops;
	case QUEEN: return color == Color::WHITE ? white_queens : black_queens;
	case KING: return color == Color::WHITE ? white_kings : black_kings;
	}
	//return *boards[color][piece];
}

int Position::moveCount() {
	return history.size();
}

Move Position::lastMove() {
	return history.back();
}

u64* Position::boardReference(Square square) {
	if ((white_pawns & BoardMask::board(square)) != 0) return &white_pawns;
	if ((white_rooks & BoardMask::board(square)) != 0) return &white_rooks;
	if ((white_knights & BoardMask::board(square)) != 0) return &white_knights;
	if ((white_bishops & BoardMask::board(square)) != 0) return &white_bishops;
	if ((white_queens & BoardMask::board(square)) != 0) return &white_queens;
	if ((white_kings & BoardMask::board(square)) != 0) return &white_kings;
	if ((black_pawns & BoardMask::board(square)) != 0) return &black_pawns;
	if ((black_rooks & BoardMask::board(square)) != 0) return &black_rooks;
	if ((black_knights & BoardMask::board(square)) != 0) return &black_knights;
	if ((black_bishops & BoardMask::board(square)) != 0) return &black_bishops;
	if ((black_queens & BoardMask::board(square)) != 0) return &black_queens;
	if ((black_kings & BoardMask::board(square)) != 0) return &black_kings;
	return nullptr;
}

u64* Position::boardReference(Color color, Piece piece) {
	if (color == Color::WHITE) {
		if (piece == KNIGHT) return &white_knights;
		if (piece == BISHOP) return &white_bishops;
		if (piece == ROOK) return &white_rooks;
		if (piece == QUEEN) return &white_queens;
		if (piece == KING) return &white_kings;
		if (piece == PAWN) return &white_pawns;
	}
	else {
		if (piece == KNIGHT) return &black_knights;
		if (piece == BISHOP) return &black_bishops;
		if (piece == ROOK) return &black_rooks;
		if (piece == QUEEN) return &black_queens;
		if (piece == KING) return &black_kings;
		if (piece == PAWN) return &black_pawns;
	}
	//return boards[color][piece];
}

void Position::makeMove(Move move) {
	if (castling.white_kcastle == true) move.castlingRights += 1;
	if (castling.white_qcastle == true) move.castlingRights += 2;
	if (castling.black_kcastle == true) move.castlingRights += 4;
	if (castling.black_qcastle == true) move.castlingRights += 8;

	if (move.isCapture()) {
		if (move.isEnPassant()) {
			move.capturedSquare = (Square)(turnColor == Color::BLACK ? (move.to() + 8) : (move.to() - 8));
		}
		else {
			move.capturedSquare = move.to();
		}

		zobristKey ^= trans.seedMap[move.capturedSquare][(Color)-turnColor][getPiece(move.capturedSquare)];
		u64* captured = boardReference(move.capturedSquare);
		*captured &= ~BoardMask::board(move.capturedSquare);
		move.capture = captured; 

		if (captured == &white_rooks && move.to() == S_H1) castling.white_kcastle = false;
		if (captured == &white_rooks && move.to() == S_A1) castling.white_qcastle = false;
		if (captured == &black_rooks && move.to() == S_H8) castling.black_kcastle = false;
		if (captured == &black_rooks && move.to() == S_A8) castling.black_qcastle = false;
	}

	zobristKey ^= trans.seedMap[move.from()][turnColor][getPiece(move.from())];
	u64* board = boardReference(move.from());
	*board &= ~BoardMask::board(move.from());
	*board |= BoardMask::board(move.to());
	zobristKey ^= trans.seedMap[move.to()][turnColor][getPiece(move.to())];

	if (board == &white_kings) {
		castling.white_kcastle = false;
		castling.white_qcastle = false;
		if (move.isKingSideCastle()) {
			zobristKey ^= trans.seedMap[S_H1][WHITE][ROOK];
			u64* whiteRooks = boardReference(S_H1);
			*whiteRooks &= ~BoardMask::board(S_H1);
			*whiteRooks |= BoardMask::board(S_F1);
			zobristKey ^= trans.seedMap[S_F1][WHITE][ROOK];
			zobristKey ^= trans.castlingRights[0];
		}
		if (move.isQueenSideCastle()) {
			zobristKey ^= trans.seedMap[S_A1][WHITE][ROOK];
			u64* whiteRooks = boardReference(S_A1);
			*whiteRooks &= ~BoardMask::board(S_A1);
			*whiteRooks |= BoardMask::board(S_D1);
			zobristKey ^= trans.seedMap[S_D1][WHITE][ROOK];
			zobristKey ^= trans.castlingRights[1];
		}
	}

	if (board == &black_kings) {
		castling.black_kcastle = false;
		castling.black_qcastle = false;
		if (move.isKingSideCastle()) {
			zobristKey ^= trans.seedMap[S_H8][BLACK][ROOK];
			u64* blackRooks = boardReference(S_H8);
			*blackRooks &= ~BoardMask::board(S_H8);
			*blackRooks |= BoardMask::board(S_F8);
			zobristKey ^= trans.seedMap[S_F8][BLACK][ROOK];
			zobristKey ^= trans.castlingRights[2];
		}
		if (move.isQueenSideCastle()) {
			zobristKey ^= trans.seedMap[S_A8][BLACK][ROOK];
			u64* blackRooks = boardReference(S_A8);
			*blackRooks &= ~BoardMask::board(S_A8);
			*blackRooks |= BoardMask::board(S_D8);
			zobristKey ^= trans.seedMap[S_D8][BLACK][ROOK];
			zobristKey ^= trans.castlingRights[3];
		}
	}

	if (board == &white_rooks && move.from() == S_H1) castling.white_kcastle = false;
	if (board == &white_rooks && move.from() == S_A1) castling.white_qcastle = false;
	if (board == &black_rooks && move.from() == S_H8) castling.black_kcastle = false;
	if (board == &black_rooks && move.from() == S_A8) castling.black_qcastle = false;

	if (move.isPromotion()) {
		zobristKey ^= trans.seedMap[move.to()][turnColor][PAWN];
		*board &= ~BoardMask::board(move.to());
		*boardReference(turnColor, move.getPromotion()) |= BoardMask::board(move.to());
		zobristKey ^= trans.seedMap[move.to()][turnColor][move.getPromotion()];
	}

	history.push_back(move);
	turnColor = turnColor == Color::WHITE ? Color::BLACK : Color::WHITE;
	zobristKey ^= trans.blackMoves;
}

void Position::unMakeMove() {
	turnColor = turnColor == Color::WHITE ? Color::BLACK : Color::WHITE;
	zobristKey ^= trans.blackMoves;
	Move move = lastMove();

	if (move.isPromotion()) {
		zobristKey ^= trans.seedMap[move.to()][turnColor][move.getPromotion()];
		*boardReference(turnColor, move.getPromotion()) &= ~BoardMask::board(move.to());
		*boardReference(turnColor, PAWN) |= BoardMask::board(move.to());
		zobristKey ^= trans.seedMap[move.to()][turnColor][PAWN];
	}

	zobristKey ^= trans.seedMap[move.to()][turnColor][getPiece(move.to())];
	u64* board = boardReference(move.to());
	*board &= ~BoardMask::board(move.to());
	*board |= BoardMask::board(move.from());
	zobristKey ^= trans.seedMap[move.from()][turnColor][getPiece(move.from())];

	if (board == &white_kings) {
		if (move.isKingSideCastle()) {
			zobristKey ^= trans.seedMap[S_F1][WHITE][ROOK];
			u64* whiteRooks = boardReference(S_F1);
			*whiteRooks &= ~BoardMask::board(S_F1);
			*whiteRooks |= BoardMask::board(S_H1);
			zobristKey ^= trans.seedMap[S_H1][WHITE][ROOK];
			zobristKey ^= trans.castlingRights[0];
		}
		if (move.isQueenSideCastle()) {
			zobristKey ^= trans.seedMap[S_D1][WHITE][ROOK];
			u64* whiteRooks = boardReference(S_D1);
			*whiteRooks &= ~BoardMask::board(S_D1);
			*whiteRooks |= BoardMask::board(S_A1);
			zobristKey ^= trans.seedMap[S_A1][WHITE][ROOK];
			zobristKey ^= trans.castlingRights[1];
		}
	}

	if (board == &black_kings) {
		if (move.isKingSideCastle()) {
			zobristKey ^= trans.seedMap[S_F8][BLACK][ROOK];
			u64* blackRooks = boardReference(S_F8);
			*blackRooks &= ~BoardMask::board(S_F8);
			*blackRooks |= BoardMask::board(S_H8);
			zobristKey ^= trans.seedMap[S_H8][BLACK][ROOK];
			zobristKey ^= trans.castlingRights[2];
		}
		if (move.isQueenSideCastle()) {
			zobristKey ^= trans.seedMap[S_D8][BLACK][ROOK];
			u64* blackRooks = boardReference(S_D8);
			*blackRooks &= ~BoardMask::board(S_D8);
			*blackRooks |= BoardMask::board(S_A8);
			zobristKey ^= trans.seedMap[S_A8][BLACK][ROOK];
			zobristKey ^= trans.castlingRights[3];
		}
	}

	if (move.isCapture()) {
		*move.capture |= BoardMask::board(move.capturedSquare);
		zobristKey ^= trans.seedMap[move.capturedSquare][(Color)-turnColor][getPiece(move.capturedSquare)];
	}

	castling.white_kcastle = (move.castlingRights & 1) == 0 ? false : true;
	castling.white_qcastle = (move.castlingRights & 2) == 0 ? false : true;
	castling.black_kcastle = (move.castlingRights & 4) == 0 ? false : true;
	castling.black_qcastle = (move.castlingRights & 8) == 0 ? false : true;

	history.pop_back();
}

u64 Position::zobristHash() {
	u64 zobristHash = 0ULL;

	unsigned long idx;
	u64 board = 0;

	for (Color color : ColorList) {
		for (Piece piece : PieceList) {
			board = getBoard(color, piece);
			while (board != 0) {
				_BitScanForward64(&idx, board);
				zobristHash ^= trans.seedMap[(Square)idx][color][piece];
				board ^= 0x1ULL << idx;
			}
		}
	}

	if (turnColor == Color::BLACK) zobristHash ^= trans.blackMoves;
	if (castling.white_kcastle) zobristHash ^= trans.castlingRights[0];
	if (castling.white_qcastle) zobristHash ^= trans.castlingRights[1];
	if (castling.black_kcastle) zobristHash ^= trans.castlingRights[2];
	if (castling.black_qcastle) zobristHash ^= trans.castlingRights[3];

	if (moveCount() > 0 && lastMove().isDoublePawnPush()) {
		zobristHash ^= trans.enPassantFiles[BoardMask::file(lastMove().to())];
	}

	return zobristHash;
}

void Position::print() {
	char board[8][8];

	for (size_t x = 0; x < 8; ++x) {
		for (size_t y = 0; y < 8; ++y) {
			int idx = 63 - (x * 8 + y);
			u64 boardMask = BoardMask::board((Square) idx);

			if ((boardMask & white_pawns) != 0) board[x][y] = 'P';
			else if ((boardMask & white_knights) != 0) board[x][y] = 'N';
			else if ((boardMask & white_bishops) != 0) board[x][y] = 'B';
			else if ((boardMask & white_rooks) != 0) board[x][y] = 'R';
			else if ((boardMask & white_queens) != 0) board[x][y] = 'Q';
			else if ((boardMask & white_kings) != 0) board[x][y] = 'K';
			else if ((boardMask & black_pawns) != 0) board[x][y] = 'p';
			else if ((boardMask & black_knights) != 0) board[x][y] = 'n';
			else if ((boardMask & black_bishops) != 0) board[x][y] = 'b';
			else if ((boardMask & black_rooks) != 0) board[x][y] = 'r';
			else if ((boardMask & black_queens) != 0) board[x][y] = 'q';
			else if ((boardMask & black_kings) != 0) board[x][y] = 'k';
			else board[x][y] = ' ';
		}
	}

	for (size_t x = 0; x < 8; ++x) {
		std::cout << x + 1 << " | ";
		for (size_t y = 0; y < 8; ++y) {
			std::cout << board[x][y] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "    ---------------" << std::endl;
	std::cout << "    A B C D E F G H" << std::endl;
	std::cout << (turnColor == Color::WHITE ? "White" : "Black") << " to move." << std::endl;
}
