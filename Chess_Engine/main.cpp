#include "position.h"
#include "moveGen.h"
#include <chrono>

int main() {
	Position* position = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	MoveGen& gen = *(new MoveGen(*position));
	
	for (int i = 0; i < 6; i++) {
		auto start = std::chrono::high_resolution_clock::now();

		std::cout << gen.perft(i, i) << std::endl;

		auto stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = stop - start;
		std::cout << "Elapsed time: " << elapsed.count() << " s\n";
	}
	
	//	
	//gen.genMoves();

	//for (Move m : gen.moves) {
	//	m.print();
	//}

	//std::cout << gen.moves.size() << std::endl;
}