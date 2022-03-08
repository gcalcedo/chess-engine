#include "bitboard.h"
#include "moveGen.h"

int main() {
	BitBoard *bitboard = new BitBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	MoveGen &gen = *(new MoveGen(*bitboard, Color::WHITE));

	gen.genPawn();
}