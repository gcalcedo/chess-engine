#include "position.h"

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
