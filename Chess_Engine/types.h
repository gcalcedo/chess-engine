#pragma once

typedef unsigned long long U64;

enum class Color {
	WHITE,
	BLACK
};

enum Direction {
	NORTH = 8,
	EAST = -1,
	SOUTH = -NORTH,
	WEST = -EAST,

	NORTH_EAST = NORTH + EAST,
	NORTH_WEST = NORTH + WEST,
	SOUTH_EAST = SOUTH + EAST,
	SOUTH_WEST = SOUTH + WEST,

	NORTH2 = NORTH + NORTH,
	SOUTH2 = SOUTH + SOUTH,

	NORTH2_EAST = NORTH2 + EAST,
	NORTH2_WEST = NORTH2 + WEST,
	SOUTH2_EAST = SOUTH2 + EAST,
	SOUTH2_WEST = SOUTH2 + WEST
};

enum File : U64 {
	FILE_A = 0x8080808080808080ULL,
	FILE_B = FILE_A >> 1,
	FILE_C = FILE_A >> 2,
	FILE_D = FILE_A >> 3,
	FILE_E = FILE_A >> 4,
	FILE_F = FILE_A >> 5,
	FILE_G = FILE_A >> 6,
	FILE_H = FILE_A >> 7
};

enum Rank : U64 {
	RANK_1 = 0x00000000000000FFULL,
	RANK_2 = RANK_1 << 8 * 1,
	RANK_3 = RANK_1 << 8 * 2,
	RANK_4 = RANK_1 << 8 * 3,
	RANK_5 = RANK_1 << 8 * 4,
	RANK_6 = RANK_1 << 8 * 5,
	RANK_7 = RANK_1 << 8 * 6,
	RANK_8 = RANK_1 << 8 * 7
};
