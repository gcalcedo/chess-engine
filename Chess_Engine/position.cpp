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

void Position::makeMove(Move move) {
	if (move.isCapture()) {
		if (move.isEnPassant()) {
			move.capturedSquare = (Square) (turn == Color::BLACK ? (move.to() + 8) : (move.to() - 8));
		}
		else {
			move.capturedSquare = move.to();
		}

		u64* captured = boardReference(move.capturedSquare);
		*captured &= ~BoardMask::board(move.capturedSquare);
		move.capture = captured;
	}

	u64* board = boardReference(move.from());
	*board &= ~BoardMask::board(move.from());
	*board |= BoardMask::board(move.to());

	history.push_back(move);
	turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void Position::unMakeMove() {
	Move move = lastMove();

	u64* board = boardReference(move.to());
	*board &= ~BoardMask::board(move.to());
	*board |= BoardMask::board(move.from());

	if (move.isCapture()) {
		*move.capture |= BoardMask::board(move.capturedSquare);
	}

	history.pop_back();
	turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;
}
