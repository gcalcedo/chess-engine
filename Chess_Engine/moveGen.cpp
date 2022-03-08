#include "moveGen.h"

MoveGen::MoveGen(BitBoard &position, Color turn) : pos(position) {
	this->turn = turn;
}

void MoveGen::genPawn() {
	pos.printBitBoard(pos.white_pawns);
}