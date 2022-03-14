#pragma once

#include <string>

enum Square;
enum Piece;

static class Stringify {
public:
	static std::string square(Square square) {
		switch (square % 8) {
		case 0: return "H" + std::to_string((square / 8) + 1);
		case 1: return "G" + std::to_string((square / 8) + 1);
		case 2: return "F" + std::to_string((square / 8) + 1);
		case 3: return "E" + std::to_string((square / 8) + 1);
		case 4: return "D" + std::to_string((square / 8) + 1);
		case 5: return "C" + std::to_string((square / 8) + 1);
		case 6: return "B" + std::to_string((square / 8) + 1);
		case 7: return "A" + std::to_string((square / 8) + 1);
		}
	}

	static std::string piece(Piece piece) {
		switch (piece) {
		case 0: return "ROOK";
		case 1: return "KNIGHT";
		case 2: return "BISHOP";
		case 3: return "QUEEN";
		case 4: return "KING";
		case 5: return "PAWN";
		}
	}

	static std::string bitboard(unsigned long long bitboard) {
		std::string stringBitboard = "";

		for (size_t x = 0; x < 8; ++x) {
			stringBitboard.append(std::to_string(8 - x) + " | ");
			for (size_t y = 0; y < 8; ++y) {
				int idx = 63 - (x * 8 + y);

				if ((1ll << idx) & bitboard) {
					stringBitboard.append("1 ");
				}
				else {
					stringBitboard.append("0 ");
				}
			}
			stringBitboard.append("\n");
		}
		stringBitboard.append("   -----------------\n");
		stringBitboard.append("    A B C D E F G H");

		return stringBitboard;
	}
};
