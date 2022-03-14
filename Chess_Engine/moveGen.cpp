#include "moveGen.h"
#include <chrono>

MoveGen::MoveGen(Position& position) : pos(position) {}

void MoveGen::genMoves(Color color) {
	moves.clear();

	turn = color;

	player = turn == Color::WHITE ? white_mask() : black_mask();
	rival = turn == Color::WHITE ? black_mask() : white_mask();
	occupied = player | rival;
	empty = ~occupied;

	auto start = std::chrono::high_resolution_clock::now();
	genPawn();
	genKnight();
	genRook();
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = stop - start;

	for (Move move : moves) {
		move.print();
	}

	std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	/*
	std::string board =
		"01000000"
		"00100000"
		"00010000"
		"00001000"
		"00000100"
		"00000010"
		"00000001"
		"00000000";

	std::cout << std::stoull(board, nullptr, 2) << std::endl;
	std::cout << RANK_5 << std::endl;
	*/
}

u64 MoveGen::white_mask() {
	return pos.white_pawns | pos.white_rooks | pos.white_knights | pos.white_bishops | pos.white_queens | pos.white_kings;
}

u64 MoveGen::black_mask() {
	return pos.black_pawns | pos.black_rooks | pos.black_knights | pos.black_bishops | pos.black_queens | pos.black_kings;
}

void MoveGen::genPawn() {
	u64 pawns = pos.getBoard(turn, PAWN);

	//Capture
	appendMoves(move<NORTH_EAST>(pawns) & rival & ~RANK_8, SOUTH_WEST, CAPTURE);
	appendMoves(move<NORTH_WEST>(pawns) & rival & ~RANK_8, SOUTH_EAST, CAPTURE);

	//Push
	appendMoves(move<NORTH>(pawns) & empty & ~RANK_8, SOUTH, QUIET);
	appendMoves(move<NORTH>(empty) & (move<NORTH2>(pawns & RANK_2) & empty), SOUTH2, DOUBLE_PAWN_PUSH);

	//Promotion
	appendPromotions(move<NORTH_EAST>(pawns) & rival & RANK_8, SOUTH_WEST, CAPTURE);
	appendPromotions(move<NORTH_WEST>(pawns) & rival & RANK_8, SOUTH_EAST, CAPTURE);
	appendPromotions(move<NORTH>(pawns) & empty & RANK_8, SOUTH, QUIET);
}

void MoveGen::genKnight() {
	u64 knights = pos.getBoard(turn, KNIGHT);

	appendMoves(move<NORTH2_EAST>(knights) & empty, SOUTH2_WEST, QUIET);
	appendMoves(move<NORTH2_WEST>(knights) & empty, SOUTH2_EAST, QUIET);
	appendMoves(move<SOUTH2_EAST>(knights) & empty, NORTH2_WEST, QUIET);
	appendMoves(move<SOUTH2_WEST>(knights) & empty, NORTH2_EAST, QUIET);
	appendMoves(move<EAST2_NORTH>(knights) & empty, WEST2_SOUTH, QUIET);
	appendMoves(move<EAST2_SOUTH>(knights) & empty, WEST2_NORTH, QUIET);
	appendMoves(move<WEST2_NORTH>(knights) & empty, EAST2_SOUTH, QUIET);
	appendMoves(move<WEST2_SOUTH>(knights) & empty, EAST2_NORTH, QUIET);

	appendMoves(move<NORTH2_EAST>(knights) & rival, SOUTH2_WEST, CAPTURE);
	appendMoves(move<NORTH2_WEST>(knights) & rival, SOUTH2_EAST, CAPTURE);
	appendMoves(move<SOUTH2_EAST>(knights) & rival, NORTH2_WEST, CAPTURE);
	appendMoves(move<SOUTH2_WEST>(knights) & rival, NORTH2_EAST, CAPTURE);
	appendMoves(move<EAST2_NORTH>(knights) & rival, WEST2_SOUTH, CAPTURE);
	appendMoves(move<EAST2_SOUTH>(knights) & rival, WEST2_NORTH, CAPTURE);
	appendMoves(move<WEST2_NORTH>(knights) & rival, EAST2_SOUTH, CAPTURE);
	appendMoves(move<WEST2_SOUTH>(knights) & rival, EAST2_NORTH, CAPTURE);
}

void MoveGen::genRook() {
	u64 rooks = pos.getBoard(turn, ROOK);

	unsigned long idx;
	while (rooks != 0) {
		_BitScanForward64(&idx, rooks);
		std::cout << Stringify::bitboard(rookLines(0x1ULL << idx)) << std::endl;
		rooks ^= 1ULL << idx;
	}
}


template <typename T, T m, int k>
static inline T swapbits(T p) {
	T q = ((p >> k) ^ p) & m;
	return p ^ q ^ (q << k);
}

uint64_t rev(uint64_t n)
{
	static const uint64_t m1 = ((uint64_t(1) << 63) - 1) / (1 + (1 << 1) + (1 << 2));
	static const uint64_t m2 = ((uint64_t(1) << 63) - 1) / (1 + (1 << 3) + (1 << 6));
	static const uint64_t m3 = ((uint64_t(1) << 9) - 1) + (((uint64_t(1) << 9) - 1) << 36);
	static const uint64_t m4 = (uint64_t(1) << 27) - 1;
	n = swapbits<uint64_t, m1, 2>(n);
	n = swapbits<uint64_t, m2, 6>(n);
	n = swapbits<uint64_t, m3, 18>(n);
	n = swapbits<uint64_t, m4, 36>(n);
	n = (n >> 63) | (n << 1);
	return n;
}

u64 MoveGen::rookLines(u64 piece) {
	u64 lines = 0x0ULL;

	u64 horizontal = ((occupied - 2 * piece) ^ rev(rev(occupied) - 2 * rev(piece))) & RANK_5;
	u64 vertical = (((occupied & FILE_E) - (2 * piece)) ^ rev(rev(occupied & FILE_E) - 2 * rev(piece))) & FILE_E;

	std::cout << Stringify::bitboard(horizontal) << std::endl;
	std::cout << Stringify::bitboard(vertical) << std::endl;

	lines = (horizontal | vertical) & ~player;

	return lines;
}


void MoveGen::appendMoves(u64 source, Direction move, char moveFlags) {
	unsigned long idx;
	while (source != 0) {
		_BitScanForward64(&idx, source);
		moves.push_back(Move(Square(idx + move), (Square)idx, moveFlags));
		source ^= 1ULL << idx;
	}
}

void MoveGen::appendPromotions(u64 source, Direction move, char moveFlags) {
	unsigned long idx;
	while (source != 0) {
		_BitScanForward64(&idx, source);
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | ROOK | moveFlags));
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | KNIGHT | moveFlags));
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | BISHOP | moveFlags));
		moves.push_back(Move(Square(idx + move), (Square)idx, PROMOTION | QUEEN | moveFlags));
		source ^= 1ULL << idx;
	}
}