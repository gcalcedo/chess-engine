#include "bitboard.h"

BitBoard::BitBoard(std::string fen) {
	printBoard();
	init();
}

void BitBoard::init() {
	std::string binary;

	for (size_t x = 0; x < 8; ++x) {
		for (size_t y = 0; y < 8; ++y) {
			binary = "0000000000000000000000000000000000000000000000000000000000000000";
			binary = binary.substr(x * 8 + y + 1) + "1" + binary.substr(0, x * 8 + y);

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

void BitBoard::printBoard() {
	for (size_t x = 0; x < 8; ++x) {
		for (size_t y = 0; y < 8; ++y) {
			std::cout << board[x][y] << " ";
		}
		std::cout << "\n";
	}
}

void BitBoard::printBitBoard(long long value) {
	for (size_t x = 0; x < 8; ++x) {
		for (size_t y = 0; y < 8; ++y) {
			int idx = x * 8 + y;

			if ((1ll << idx) & value) {
				std::cout << "1 ";
			}
			else {
				std::cout << "0 ";
			}
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}
