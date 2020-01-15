 #include <stack>
#include <iostream>
#include<Windows.h>

#include "Mouse.h"
#include "CustomMaze.h"
#include "Branch.h"

const char ROUTE_CHAR = '-';
const char DEADEND_CHAR = 'x';
const char MOUSE_CHAR = 'M';

// Moves the mouse to the passed tiles coordinates in the given maze
bool Mouse::MoveTo(const Tile& toTile, CustomMaze &maze) 
{
	if (toTile.displayChar == maze.getFloorChar() || toTile.displayChar == maze.getStartChar() || toTile.isExit)
	{
		lastPos = pos;
		pos = toTile.pos;
		maze.grid[pos.x][pos.y].isExplored = true;

		return true;
	}
	else
	{
		return false;
	}
}

// Prints the character M to the console at the mouse's position
void Mouse::PrintPosition(CustomMaze &maze, std::stack<Branch> &branchStack)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO buffer;
	COORD lastOutputPos{ 0, 0 };

	if (GetConsoleScreenBufferInfo(output, &buffer))
	{
		lastOutputPos = buffer.dwCursorPosition;
	}

	COORD outputPos;

	if (maze.grid[lastPos.x][lastPos.y].displayChar == maze.getFloorChar())
	{
		outputPos = { static_cast<short>(lastPos.x), static_cast<short>(lastPos.y) };
		SetConsoleCursorPosition(output, outputPos);

		if (isRetracingSteps)
		{
			std::cout << DEADEND_CHAR;
		}
		else
		{
			std::cout << ROUTE_CHAR;
		}
	}

	outputPos = { static_cast<short>(pos.x), static_cast<short>(pos.y) };
	SetConsoleCursorPosition(output, outputPos);
	std::cout << MOUSE_CHAR;

	SetConsoleCursorPosition(output, lastOutputPos);

	return;
}

// Returns the tile currently to the mouse's right
Tile Mouse::GetRightTile(Mouse &mouse, CustomMaze &maze)
{
	return maze.grid[pos.x + 1][pos.y];
}

// Returns the tile currently underneath the mouse
Tile Mouse::GetDownTile(Mouse &mouse, CustomMaze &maze)
{
	return maze.grid[pos.x][pos.y + 1];
}

// Returns the tile to the mouse's left
Tile Mouse::GetLeftTile(Mouse &mouse, CustomMaze &maze)
{
	return maze.grid[pos.x - 1][pos.y];
}

//Returns the tile currently above the mouse
Tile Mouse::GetUpTile(Mouse &mouse, CustomMaze &maze)
{
	return maze.grid[pos.x][pos.y - 1];
}

// Returns true if the mouse can move to the tile currently to the mouse's right
bool Mouse::CanMoveRight(Mouse &mouse, CustomMaze &maze)
{
	if (maze.getFloorChar() == GetRightTile(mouse, maze).displayChar)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Returns true if the mouse can move to the tile currently underneath the mouse
bool Mouse::CanMoveDown(Mouse &mouse, CustomMaze &maze)
{
	if (maze.getFloorChar() == GetDownTile(mouse, maze).displayChar)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Returns true if the mouse can move to the tile currently to the mouse's left
bool Mouse::CanMoveLeft(Mouse &mouse, CustomMaze &maze)
{
	if (maze.getFloorChar() == GetLeftTile(mouse, maze).displayChar)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Returns true if the mouse can move to the tile currently above the mouse
bool Mouse::CanMoveUp(Mouse &mouse, CustomMaze &maze)
{
	if (maze.getFloorChar() == GetUpTile(mouse, maze).displayChar)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Returns true if the mouse has explored the right tile
bool Mouse::IsRightTileExplored(Mouse &mouse, CustomMaze &maze)
{
	if (GetRightTile(mouse, maze).isExplored)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Returns true if the mouse has explored the below tile
bool Mouse::IsDownTileExplored(Mouse &mouse, CustomMaze &maze)
{
	if (GetDownTile(mouse, maze).isExplored)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Returns true if the mouse has explored the left tile
bool Mouse::IsLeftTileExplored(Mouse &mouse, CustomMaze &maze)
{
	if (GetLeftTile(mouse, maze).isExplored)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Returns true if the mouse has explored the above tile
bool Mouse::IsUpTileExplored(Mouse &mouse, CustomMaze &maze)
{
	if (GetUpTile(mouse, maze).isExplored)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Moves the mouse to the exit tile passed. Only use when moving to the exit tile.
bool Mouse::MoveToExit(const Tile &toTile, CustomMaze &maze, std::stack<Branch> &branchStack)
{
	if (MoveTo(toTile, maze))
	{
		branchStack.top().tiles.push(toTile);
		hasFoundCheese = true;
		return true;
	}
	else 
	{
		return false;
	}
}

// Returns true if the mouse can move to any tiles that it hasn't explored before. 
// If true, the mouse will move to the next available tile prioritising right, then down, then left, then up.
bool Mouse::ExploreMaze(Mouse &mouse, CustomMaze &maze, std::stack<Branch> &branchStack)
{
	Tile rightTile = GetRightTile(mouse, maze);
	Tile downTile = GetDownTile(mouse, maze);
	Tile leftTile = GetLeftTile(mouse, maze);
	Tile upTile = GetUpTile(mouse, maze);

	if (maze.getExitChar() == rightTile.displayChar)
	{
		MoveToExit(rightTile, maze, branchStack);
	}
	else if (maze.getExitChar() == downTile.displayChar)
	{
		MoveToExit(downTile, maze, branchStack);
	}
	else if (maze.getExitChar() == leftTile.displayChar)
	{
		MoveToExit(leftTile, maze, branchStack);
	}
	else if (maze.getExitChar() == upTile.displayChar)
	{
		MoveToExit(upTile, maze, branchStack);
	}

	int paths = 0;
	int exploredPaths = 0;

	Tile nextTile;
	nextTile.isExplored = true;

	if (CanMoveUp(mouse, maze))
	{
		paths++;
		if (!IsUpTileExplored(mouse, maze))
		{
			nextTile = upTile;
		}
		else 
		{
			exploredPaths++;
		}
	}

	if (CanMoveLeft(mouse, maze))
	{
		paths++;
		if (!IsLeftTileExplored(mouse, maze))
		{
			nextTile = leftTile;
		}
		else
		{
			exploredPaths++;
		}
	}

	if (CanMoveDown(mouse, maze))
	{
		paths++;
		if (!IsDownTileExplored(mouse, maze))
		{
			nextTile = downTile;
		}
		else
		{
			exploredPaths++;
		}
	}

	if (CanMoveRight(mouse, maze))
	{
		paths++;
		if (!IsRightTileExplored(mouse, maze))
		{
			nextTile = rightTile;
		}
		else
		{
			exploredPaths++;
		}
	}

	// If mouse reaches an intersection push a new branch into stack
	if (paths > 2 && exploredPaths < paths)
	{
		Branch newBranch;
		newBranch.tiles.push(maze.grid[pos.x][pos.y]);
		branchStack.push(newBranch);
	}

	if (true == nextTile.isExplored)
	{
		return false; // Dead end reached
	}
	else
	{
		mouse.MoveTo(nextTile, maze);
		branchStack.top().tiles.push(nextTile);
		return true;
	}
}

// Moves the mouse back to the last intersection it was at
void Mouse::RetraceSteps(Mouse &mouse, CustomMaze &maze, std::stack<Branch> &branchStack)
{
	if (branchStack.top().tiles.empty())
	{
		// Top of branch reached
		branchStack.pop();
		isRetracingSteps = false;
		lastPos = pos;
		return;
	}
	else
	{
		maze.grid[pos.x][pos.y].leadsToDeadEnd = true;

		// Continue retracing steps
		isRetracingSteps = true;
		Tile toTile = branchStack.top().tiles.top();
		mouse.MoveTo(toTile, maze);
		branchStack.top().tiles.pop();
		return;
	}
}
