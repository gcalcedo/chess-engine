#pragma once

#include <string>
#include <vector>
#include "types.h"
#include "boardMask.h"

class MoveParser
{
private:
	File parseFile(char file);
	Rank parseRank(char rank);
	Square parseSquare(std::string square);
	char parseMoveFlag(std::string moveFlag);

public:
	Move parseMove(std::string moveString);
};

