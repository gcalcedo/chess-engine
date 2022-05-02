#include "position.h"
#include "moveGen.h"
#include "evaluation.h"
#include "search.h"
#include <chrono>

int main() {
	Position* position = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	MoveGen& gen = *(new MoveGen(*position));
	Evaluation& eval = *(new Evaluation(*position));

	Search& search = *(new Search(*position, gen, eval));

	//std::vector<Move> moves = gen.genMoves();

	//while (moves.size() > 0) {
	//	search.search(7);
	//	search.bestMove.print();
	//	position->makeMove(search.bestMove);
	//}

	position->makeMove(Move(S_C2, S_C4, QUIET));
	position->makeMove(Move(S_H7, S_H6, QUIET));
	position->makeMove(Move(S_C4, S_C5, QUIET));
	position->makeMove(Move(S_H6, S_H5, QUIET));
	position->makeMove(Move(S_G2, S_G3, QUIET));
	position->makeMove(Move(S_H5, S_H4, QUIET));
	position->makeMove(Move(S_G3, S_H4, CAPTURE));
	position->makeMove(Move(S_E7, S_E6, QUIET));
	position->makeMove(Move(S_E2, S_E4, QUIET));
	position->makeMove(Move(S_D8, S_H4, CAPTURE));
	position->makeMove(Move(S_G1, S_F3, QUIET));
	position->makeMove(Move(S_H4, S_E4, CAPTURE));
	position->makeMove(Move(S_F1, S_E2, QUIET));
	position->makeMove(Move(S_G7, S_G6, QUIET));
	position->makeMove(Move(S_D2, S_D3, QUIET));
	position->makeMove(Move(S_E4, S_B4, QUIET));
	position->makeMove(Move(S_C1, S_D2, QUIET));
	position->makeMove(Move(S_B4, S_C5, CAPTURE));
	position->makeMove(Move(S_D3, S_D4, QUIET));
	position->makeMove(Move(S_C5, S_H5, QUIET));
	position->makeMove(Move(S_D2, S_F4, QUIET));
	position->makeMove(Move(S_H5, S_G4, QUIET));
	position->makeMove(Move(S_D1, S_D2, QUIET));
	position->makeMove(Move(S_G4, S_G2, QUIET));
	position->makeMove(Move(S_H1, S_G1, QUIET));
	position->makeMove(Move(S_G2, S_H3, QUIET));
	position->makeMove(Move(S_B1, S_C3, QUIET));
	position->makeMove(Move(S_F8, S_B4, QUIET));
	position->makeMove(Move(S_D4, S_D5, QUIET));
	position->makeMove(Move(S_E6, S_D5, CAPTURE));
	position->makeMove(Move(S_D2, S_E3, QUIET));
	position->makeMove(Move(S_G8, S_E7, QUIET));
	position->makeMove(Move(S_E3, S_D4, QUIET));
	position->makeMove(Move(S_C7, S_C5, DOUBLE_PAWN_PUSH));
	position->makeMove(Move(S_D4, S_E5, QUIET));
	position->makeMove(Move(S_D5, S_D4, QUIET));
	position->makeMove(Move(S_G1, S_G3, QUIET));
	position->makeMove(Move(S_H3, S_H7, QUIET));
	position->makeMove(Move(S_F3, S_G5, QUIET));
	position->makeMove(Move(S_H7, S_H2, CAPTURE));
	position->makeMove(Move(S_E1, S_C1, QUEEN_SIDE_CASTLE));
	position->makeMove(Move(S_D4, S_C3, CAPTURE));
	position->makeMove(Move(S_B2, S_C3, CAPTURE));
	position->makeMove(Move(S_B8, S_C6, QUIET));
	position->makeMove(Move(S_E5, S_E4, QUIET));
	position->makeMove(Move(S_F7, S_F5, DOUBLE_PAWN_PUSH));
	position->makeMove(Move(S_E4, S_E3, QUIET));
	position->makeMove(Move(S_B4, S_A3, QUIET));
	position->makeMove(Move(S_C1, S_C2, QUIET));
	position->makeMove(Move(S_C5, S_C4, QUIET));
	position->makeMove(Move(S_E2, S_C4, CAPTURE));
	position->makeMove(Move(S_B7, S_B6, QUIET));
	position->makeMove(Move(S_C4, S_F7, QUIET));
	position->makeMove(Move(S_E8, S_F8, QUIET));
	position->makeMove(Move(S_F4, S_D6, QUIET));
	position->makeMove(Move(S_F5, S_F4, QUIET));
	position->makeMove(Move(S_E3, S_F4, CAPTURE));
	position->makeMove(Move(S_H8, S_H4, QUIET));
	position->makeMove(Move(S_F4, S_F6, QUIET));
	position->makeMove(Move(S_A3, S_D6, CAPTURE));
	position->makeMove(Move(S_F6, S_D6, CAPTURE));
	position->makeMove(Move(S_H2, S_F2, CAPTURE));
	position->makeMove(Move(S_C2, S_C1, QUIET));
	position->makeMove(Move(S_C6, S_E5, QUIET));
	position->makeMove(Move(S_D6, S_E5, CAPTURE));
	position->makeMove(Move(S_H4, S_H2, QUIET));
	position->makeMove(Move(S_G3, S_G2, QUIET));
	position->makeMove(Move(S_F2, S_G2, CAPTURE));
	position->makeMove(Move(S_E5, S_H2, CAPTURE));
	position->makeMove(Move(S_G2, S_H2, CAPTURE));
	position->makeMove(Move(S_G5, S_F3, QUIET));
	position->makeMove(Move(S_H2, S_G2, QUIET));
	position->makeMove(Move(S_F3, S_E5, QUIET));
	position->makeMove(Move(S_G2, S_G5, QUIET));
	position->makeMove(Move(S_C1, S_C2, QUIET));
	position->makeMove(Move(S_G5, S_E5, CAPTURE));
	position->makeMove(Move(S_D1, S_E1, QUIET));
	position->makeMove(Move(S_E5, S_E1, CAPTURE));
	position->makeMove(Move(S_F7, S_E6, QUIET));
	position->makeMove(Move(S_E1, S_F2, QUIET));
	position->makeMove(Move(S_C2, S_B3, QUIET));
	position->makeMove(Move(S_D7, S_E6, CAPTURE));
	position->makeMove(Move(S_B3, S_B4, QUIET));
	position->makeMove(Move(S_F2, S_A2, CAPTURE));
	position->makeMove(Move(S_B4, S_B5, QUIET));
	position->makeMove(Move(S_A2, S_B1, QUIET));
	position->makeMove(Move(S_B5, S_C4, QUIET));


	search.search(7);
	search.bestMove.print();
}