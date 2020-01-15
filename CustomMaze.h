#pragma once
#include <vector>
#include <iostream>
#include <Windows.h>

#include "cMaze.h"
#include "Tile.h"
#include "Branch.h"
#include "cVector2.h"

class CustomMaze : public cMaze
{
public:
	std::vector<std::vector<Tile>> grid; // access with mazeRowStrings[COLUMN][ROW] or [x pos][y pos]

	void PrintRoute(std::stack<Branch> branchStack, const int sleepAmount);

private:
	void PrintRetracedTile(HANDLE output, cVector2 pos, int lastColour);
	std::vector<cVector2> CheckSurroundingTiles(cVector2 currentPos, HANDLE output, int lastColour);
};
