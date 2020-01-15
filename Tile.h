#pragma once
#include "cVector2.h"

struct Tile
{
public:
	cVector2 pos = cVector2(0, 0);

	bool isExit = false;
	bool isExplored = false;
	bool leadsToDeadEnd = false;

	char displayChar = 'd';

	Tile& operator=(const Tile& a)
	{
		pos = a.pos;

		isExit = a.isExit;
		isExplored = a.isExplored;
		leadsToDeadEnd = a.leadsToDeadEnd;

		displayChar = a.displayChar;

		return *this;
	}
};
