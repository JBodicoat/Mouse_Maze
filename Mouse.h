#pragma once
#include <vector>

#include "cVector2.h"
#include "Tile.h"
#include "CustomMaze.h"
#include "Branch.h"

class Mouse
{
public:
	bool MoveTo(const Tile& toCell, CustomMaze &maze);
	bool MoveToExit(const Tile &toCell, CustomMaze &maze, std::stack<Branch> &branchStack);
	cVector2 GetPos() { return pos; }
	cVector2 GetLastPos() { return lastPos; }
	bool FoundCheese() { return hasFoundCheese; }

	void RetraceSteps(Mouse &mouse, CustomMaze &maze, std::stack<Branch> &branchStack);
	bool ExploreMaze(Mouse &mouse, CustomMaze &maze, std::stack<Branch> &branchStack);

	bool isRetracingSteps = false;

	void PrintPosition(CustomMaze &maze, std::stack<Branch> &branchStack);

private:
	cVector2 pos{ 0, 0 };
	cVector2 lastPos{ 0, 0 };

	bool hasFoundCheese{ false };

	Tile GetRightTile(Mouse &mouse, CustomMaze &maze);
	Tile GetDownTile(Mouse &mouse, CustomMaze &maze);
	Tile GetLeftTile(Mouse &mouse, CustomMaze &maze);
	Tile GetUpTile(Mouse &mouse, CustomMaze &maze);

	bool CanMoveRight(Mouse &mouse, CustomMaze &maze);
	bool CanMoveDown(Mouse &mouse, CustomMaze &maze);
	bool CanMoveLeft(Mouse &mouse, CustomMaze &maze);
	bool CanMoveUp(Mouse &mouse, CustomMaze &maze);

	bool IsRightTileExplored(Mouse &mouse, CustomMaze &maze);
	bool IsDownTileExplored(Mouse &mouse, CustomMaze &maze);
	bool IsLeftTileExplored(Mouse &mouse, CustomMaze &maze);
	bool IsUpTileExplored(Mouse &mouse, CustomMaze &maze);
};