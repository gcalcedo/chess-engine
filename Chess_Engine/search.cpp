#include "search.h"
#include "stringify.h"

Search::Search(Position& position, MoveGen& generation, Evaluation& evaluation)
	: pos(position), gen(generation), evaluation(evaluation) {};

int Search::search(int depth) {
	nodesSearched = 0;
	return negamax(depth);
}

void Search::searchTimed(int milliseconds) {
	onTime = true;
	begin = std::chrono::steady_clock::now();
	max = milliseconds;

	int depth = 0;
	nodesSearched = 0;
	while (onTime) {
		depthSearched = depth;
		negamax(depth);
		depth++;
	}
}

int Search::negamax(int depth, int alpha, int beta, bool registerMove) {
	nodesSearched++;

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= max) {
		onTime = false;
		return 0;
	}

	int alphaOriginal = alpha;

	Move hashMove = Move();
	u64 key = pos.zobristKey;
	TranspositionEntry* entry = pos.trans.table[key & 0xFFFF];
	if ((entry->zobristKey == key) && entry->depth >= depth) {
		hashMove = entry->move;

		if (entry->type == EXACT) {
			return entry->eval;
		}
		else if (entry->type == LOWER_BOUND) {
			alpha = std::max(alpha, entry->eval);
		}
		else if (entry->type == UPPER_BOUND) {
			beta = std::min(beta, entry->eval);
		}

		if (alpha >= beta) {
			return entry->eval;
		}
	}

	if (depth == 0) {
		//return  pos.turnColor * evaluation.evaluate();
		return quiescenceSearch();
	}

	std::vector<Move> moves = evaluation.sortMoves(gen.genMoves(), hashMove);
	//std::vector<Move> moves = gen.genMoves();
	if (moves.size() == 0) {
		return evaluation.evaluate();
	}

	Move bestMoveTmp = moves[0];
	int bestEval = INT_MIN + 1;

	for (Move m : moves) {
		pos.makeMove(m);
		int eval = -negamax(depth - 1, -beta, -alpha, false);
		pos.unMakeMove();

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= max) {
			onTime = false;
			return 0;
		}

		if (eval > bestEval) {
			bestEval = eval;
			bestMoveTmp = m;
		}
		alpha = std::max(alpha, bestEval);

		if (alpha >= beta) {
			break;
		}
	}

	entry->move = bestMoveTmp;
	entry->zobristKey = key;
	entry->eval = bestEval;
	if (bestEval <= alphaOriginal) {
		entry->type = UPPER_BOUND;
	}
	else if (bestEval >= beta) {
		entry->type = LOWER_BOUND;
	}
	else {
		entry->type = EXACT;
	}
	entry->depth = depth;

	if (registerMove) {
		bestMove = bestMoveTmp;
	}

	return bestEval;
}

int Search::quiescenceSearch(int alpha, int beta) {
	nodesSearched++;
	int eval = pos.turnColor * evaluation.evaluate();

	if (eval >= beta) {
		return beta;
	}
	if (alpha < eval) {
		alpha = eval;
	}

	std::vector<Move> moves = evaluation.sortMoves(gen.genMoves(true), Move());
	for (Move m : moves) {
		pos.makeMove(m);
		int eval = -quiescenceSearch(-beta, -alpha);
		pos.unMakeMove();

		if (eval >= beta) {
			return beta;
		}
		if (eval > alpha) {
			alpha = eval;
		}
	}

	return alpha;
}