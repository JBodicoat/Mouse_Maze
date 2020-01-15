#include <vector>
#include <iostream>
#include <Windows.h>

#include "cMaze.h"
#include "Tile.h"
#include "Branch.h"
#include "cVector2.h"
#include "CustomMaze.h"

const int SOLID_WHITE = 255;
const int SOLID_RED = 204;
const int SOLID_GREEN = 170;

const char ROUTE_CHAR = '-';
const char DEADEND_CHAR = 'x';

// Prints the fastest route through the maze and the dead ends explored by the mouse to the console
void CustomMaze::PrintRoute(std::stack<Branch> branchStack, const int sleepAmount)
{
	// Get the cursors info and save to reset later
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD outputPos = { 0, 0 };

	CONSOLE_SCREEN_BUFFER_INFO buffer;
	COORD lastOutputPos{ 0, 0 };
	int lastColour = 0;

	if (GetConsoleScreenBufferInfo(output, &buffer))
	{
		lastOutputPos = buffer.dwCursorPosition;
		lastColour = buffer.wAttributes;
	}

	cVector2 currentPos;
	std::vector<cVector2> positionsToCheck;

	// Break loop when all explored tiles have been printed
	while (!positionsToCheck.empty() || !branchStack.empty())
	{
		while (!positionsToCheck.empty() ||  (!branchStack.empty() && !branchStack.top().tiles.empty()))
		{
			if (!branchStack.empty() && !branchStack.top().tiles.empty())
			{
				currentPos.x = branchStack.top().tiles.top().pos.x;
				currentPos.y = branchStack.top().tiles.top().pos.y;
				outputPos = { static_cast<SHORT>(currentPos.x) , static_cast<SHORT>(currentPos.y) };
				SetConsoleCursorPosition(output, outputPos);

				SetConsoleTextAttribute(output, SOLID_WHITE);

				if (grid[currentPos.x][currentPos.y].displayChar != getStartChar() && !grid[currentPos.x][currentPos.y].isExit)
				{
					std::cout << ROUTE_CHAR;
				}

				branchStack.top().tiles.pop();

				positionsToCheck.emplace_back(currentPos);
			}
			else if (!branchStack.empty())
			{
				branchStack.pop();
			}

			Sleep(sleepAmount);

			int size = positionsToCheck.size();

			for (int i = 0; i < size; i++)
			{
				std::vector<cVector2> returnVector = CheckSurroundingTiles(positionsToCheck[i], output, lastColour);
				if (!returnVector.empty())
				{
					for (cVector2 returnCheckPos : returnVector)
					{
						positionsToCheck.emplace_back(returnCheckPos);
					}
				}
			}
			positionsToCheck.erase(positionsToCheck.begin(), positionsToCheck.begin() + size);
		}

		if (!branchStack.empty())
		{
			branchStack.pop();
			if (!branchStack.empty())
			{
				branchStack.top().tiles.pop();
			}
		}
	}

	SetConsoleTextAttribute(output, lastColour);
	SetConsoleCursorPosition(output, lastOutputPos);
}

// Prints a tile that led to a dead end
void CustomMaze::PrintRetracedTile(HANDLE output, cVector2 pos, int lastColour)
{
	COORD outputPos = { static_cast<SHORT>(pos.x), static_cast<SHORT>(pos.y) };
	SetConsoleCursorPosition(output, outputPos);
	SetConsoleTextAttribute(output, SOLID_RED);
	std::cout << DEADEND_CHAR;

	SetConsoleTextAttribute(output, lastColour);

	return;
}

// Tests if any of the surrounding tiles led to dead ends
std::vector<cVector2> CustomMaze::CheckSurroundingTiles(cVector2 currentPos, HANDLE output, int lastColour)
{
	std::vector<cVector2> tilesToCheck;

	// check right
	cVector2 rightPos = { currentPos.x + 1, currentPos.y };

	if (grid[rightPos.x][rightPos.y].leadsToDeadEnd)
	{
		grid[rightPos.x][rightPos.y].leadsToDeadEnd = false;

		PrintRetracedTile(output, rightPos, lastColour);

		tilesToCheck.emplace_back(rightPos);
	}

	//check down
	cVector2 downPos = { currentPos.x, currentPos.y + 1 };

	if (grid[downPos.x][downPos.y].leadsToDeadEnd)
	{
		grid[downPos.x][downPos.y].leadsToDeadEnd = false;

		PrintRetracedTile(output, downPos, lastColour);

		tilesToCheck.emplace_back(downPos);
	}

	//check left
	cVector2 leftPos = { currentPos.x - 1 , currentPos.y };

	if (grid[leftPos.x][leftPos.y].leadsToDeadEnd)
	{
		grid[leftPos.x][leftPos.y].leadsToDeadEnd = false;

		PrintRetracedTile(output, leftPos, lastColour);

		tilesToCheck.emplace_back(leftPos);
	}

	//check up
	cVector2 upPos = { currentPos.x, currentPos.y - 1 };

	if (grid[upPos.x][upPos.y].leadsToDeadEnd)
	{
		grid[upPos.x][upPos.y].leadsToDeadEnd = false;

		PrintRetracedTile(output, upPos, lastColour);

		tilesToCheck.emplace_back(upPos);
	}

	return tilesToCheck;
}
