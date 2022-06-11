#include "position.h"
#include "transposition.h"
#include "moveGen.h"
#include "evaluation.h"
#include "search.h"
#include "moveParser.h"
#include <chrono>

int main() {
	Position& position = *(new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
	MoveGen& gen = *(new MoveGen(position));
	Evaluation& eval = *(new Evaluation(position, gen));
	Search& search = *(new Search(position, gen, eval));

	MoveParser& parser = *(new MoveParser);

	std::string moveString;
	while (true) {
		std::getline(std::cin, moveString);
		position.makeMove(parser.parseMove(moveString));
		position.print();

		search.searchTimed(6000);
		std::cout << "Depth " << (search.depthSearched - 1) << " | " << search.nodesSearched << " nodes -> ";
		search.bestMove.print();
		position.makeMove(search.bestMove);
		position.print();
	}

	//for (int i = 1; i < 50; i++) {
	//	//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//	//search.search(i);
	//	//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//	//std::cout << "Depth " << i << " | " << search.nodesSearched << " nodes in " <<
	//	//	std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms -> ";
	//	//search.bestMove.print();


	//}

	//std::vector<Move> moves = gen.genMoves();
	//while (true) {
	//	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//	search.searchTimed(5000);
	//	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//	std::cout << "Depth " << (search.depthSearched - 1) << " | " << search.nodesSearched << " nodes in " <<
	//		std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms -> ";

	//	std::cout << "MOVE: ";
	//	search.bestMove.print();
	//	position.makeMove(search.bestMove);

	//	for (size_t i = 0; i < 65536; i++) {
	//		position.trans.table[i]->zobristKey = -1;
	//	}
	//}
}