#include "moveParser.h"

File MoveParser::parseFile(char file) {
	switch (file) {
	case 'a': return FILE_A;
	case 'b': return FILE_B;
	case 'c': return FILE_C;
	case 'd': return FILE_D;
	case 'e': return FILE_E;
	case 'f': return FILE_F;
	case 'g': return FILE_G;
	case 'h': return FILE_H;
	}
}

Rank MoveParser::parseRank(char rank) {
	switch (rank) {
	case '1': return RANK_1;
	case '2': return RANK_2;
	case '3': return RANK_3;
	case '4': return RANK_4;
	case '5': return RANK_5;
	case '6': return RANK_6;
	case '7': return RANK_7;
	case '8': return RANK_8;
	}
}

char MoveParser::parseMoveFlag(std::string flag) {
	if (flag == "dp") return DOUBLE_PAWN_PUSH;
	if (flag == "kc") return KING_SIDE_CASTLE;
	if (flag == "qc") return QUEEN_SIDE_CASTLE;
	if (flag == "c") return CAPTURE;
	if (flag == "ep") return EN_PASSANT;
	if (flag == "pr") return PROMOTION | ROOK;
	if (flag == "pk") return PROMOTION | KNIGHT;
	if (flag == "pb") return PROMOTION | BISHOP;
	if (flag == "pq") return PROMOTION | QUEEN;
}

Square MoveParser::parseSquare(std::string square) {
	File file = parseFile(square[0]);
	Rank rank = parseRank(square[1]);

	return BoardMask::square(file & rank);
}

Move MoveParser::parseMove(std::string moveString) {
	moveString += " ";
	std::string space_delimiter = " ";
	std::vector<std::string> move{};

	size_t pos = 0;
	while ((pos = moveString.find(space_delimiter)) != std::string::npos) {
		move.push_back(moveString.substr(0, pos));
		moveString.erase(0, pos + space_delimiter.length());
	}

	Square from = parseSquare(move[0]);
	Square to = parseSquare(move[1]);

	char moveFlags = 0;
	for (int i = 2; i < move.size(); i++) {
		moveFlags |= parseMoveFlag(move[i]);
	}

	return Move(from, to, moveFlags);
}