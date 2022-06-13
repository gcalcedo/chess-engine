#include "position.h"
#include "transposition.h"
#include "moveGen.h"
#include "evaluation.h"
#include "search.h"
#include "moveParser.h"
#include <chrono>
#include <iostream>

int main() {
	std::cout << "==============================" << std::endl;
	std::cout << "===        WELCOME!        ===" << std::endl;
	std::cout << "=== LET'S PLAY SOME CHESS! ===" << std::endl;
	std::cout << "==============================" << std::endl;
	std::cout << std::endl;
	std::cout << "    Start? -> Press Enter";
	getchar();

	Position& position = *(new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
	MoveGen& gen = *(new MoveGen(position));
	Evaluation& eval = *(new Evaluation(position, gen));
	Search& search = *(new Search(position, gen, eval));

	//position.castling.white_kcastle = false;
	//position.castling.white_qcastle = false;
	//position.castling.black_kcastle = false;
	//position.castling.black_qcastle = false;

	/////////////////////////////
	// PLAY AGAINST THE ENGINE //
	/////////////////////////////
	position.print();
	std::cout << "You play as White!" << std::endl;

	MoveParser& parser = *(new MoveParser);

	std::string moveString;
	while (true) {
		std::getline(std::cin, moveString);
		position.makeMove(parser.parseMove(moveString));

		position.print();
		std::cout << "You moved -> ";
		position.lastMove().print();

		std::cout << "Let me think..." << std::endl;

		search.searchTimed(5000);
		//std::cout << "Depth " << (search.depthSearched - 1) << " | " << search.nodesSearched << " nodes -> ";
		//search.bestMove.print();
		position.makeMove(search.bestMove);

		position.print();
		std::cout << "My move is -> ";
		position.lastMove().print();
		std::cout << "Your turn!" << std::endl;
	}


	//////////////////////////////////////////////
	// THE ENGINE ANALYZES THE CURRENT POSITION //
	//////////////////////////////////////////////
	//for (int i = 1; i < 50; i++) {
	//	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//	search.search(i);
	//	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//	std::cout << "Depth " << i << " | " << search.nodesSearched << " nodes in " <<
	//		std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms -> ";
	//	search.bestMove.print();
	//}

	//std::vector<Move> moves = gen.genMoves();
	//while (true) {
	//	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//	search.searchTimed(5000);
	//	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//	std::cout << "Depth " << (search.depthSearched - 1) << " | " << search.nodesSearched << " nodes in " <<
	//		std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms -> ";


	/////////////////////////////////////
	// THE ENGINE PLAYS AGAINST ITSELF //
	/////////////////////////////////////
	//	std::cout << "MOVE: ";
	//	search.bestMove.print();
	//	position.makeMove(search.bestMove);

	//	for (size_t i = 0; i < 65536; i++) {
	//		position.trans.table[i]->zobristKey = -1;
	//	}
	//}
}