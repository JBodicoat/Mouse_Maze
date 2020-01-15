#pragma once
#include<stack>
#include "Tile.h"

// Contains a stack of Tiles 
struct Branch
{
	std::stack<Tile> tiles;
};