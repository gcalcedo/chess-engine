#include "search.h"

Search::Search(Position& position, MoveGen& generation, Evaluation& evaluation)
	: pos(position), gen(generation), eval(evaluation) {};

int Search::maxValue(int depth, int alpha, int beta, bool registerMove) {
	if (depth == 0) {
		return eval.evaluate();
	}

	int bestEval = INT_MIN;

	for (Move m : gen.genMoves()) {
		pos.makeMove(m);
		int eval = minValue(depth - 1, alpha, beta, false);
		pos.unMakeMove();
		if (eval > bestEval) {
			bestEval = eval;
			if (registerMove) bestMove = m;
		}
		if (eval >= beta) return eval;
		if (eval > alpha) alpha = eval;
	}
	return bestEval;
}

int Search::minValue(int depth, int alpha, int beta, bool registerMove) {
	if (depth == 0) {
		return eval.evaluate();
	}

	int bestEval = INT_MAX;

	for (Move m : gen.genMoves()) {
		pos.makeMove(m);
		int eval = maxValue(depth - 1, alpha, beta, false);
		pos.unMakeMove();
		if (eval < bestEval) {
			bestEval = eval;
			if (registerMove) bestMove = m;
		}
		if (eval <= alpha) return eval;
		if (eval < beta) beta = eval;
	}
	return bestEval;
}

int Search::search(int depth, int alpha, int beta, bool registerMove) {
	return pos.turnColor == Color::WHITE ? maxValue(depth) : minValue(depth);
}