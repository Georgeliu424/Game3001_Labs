#pragma once
#ifndef __TILECOMPARATOR__
#define __TILECOMPARATOR__

#include "Tile.h"

class TileComparator
{
public:
	int operator()(const Tile*lhs, const Tile*rhs)const
	{
		return lhs->GetTileCost() > rhs->GetTileCost();
	}

};





#endif
