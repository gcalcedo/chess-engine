#include "position.h"
#include "moveGen.h"

int main() {
	Position *bitboard = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	MoveGen &gen = *(new MoveGen(*bitboard));

	gen.genMoves(Color::WHITE);
}