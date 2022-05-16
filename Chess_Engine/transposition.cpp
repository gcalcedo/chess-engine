#include "transposition.h"

Transposition::Transposition() {
	/* Seed */
	std::random_device rd;

	/* Random number generator */
	std::default_random_engine generator(/*rd()*/3726547162);

	/* Distribution on which to apply the generator */
	std::uniform_int_distribution<u64> distribution(0, 0xFFFFFFFFFFFFFFFF);

	for (Square square : SquareList) {
		std::unordered_map<Color, std::unordered_map<Piece, u64>> colorMap;
		for (Color color : ColorList) {
			std::unordered_map<Piece, u64> pieceMap;
			for (Piece piece : PieceList) {
				pieceMap.emplace(piece, distribution(generator));
			}
			colorMap.emplace(color, pieceMap);
		}
		seedMap.emplace(square, colorMap);
	}

	blackMoves = distribution(generator);

	for (size_t i = 0; i < 4; i++) {
		castlingRights.push_back(distribution(generator));
	}

	for (File file : FileList) {
		enPassantFiles.emplace(file, distribution(generator));
	}

	for (size_t i = 0; i < 65536; i++) {
		table[i] = (TranspositionEntry*) malloc(sizeof(TranspositionEntry));
	}
};