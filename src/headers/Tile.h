/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 4/22/2011
* File: Tile.h
* Purpose: Creates a tile for use in tile-based maps
*****************************************************************/

#ifndef TILE_INCLUDE
#define TILE_INCLUDE
#include "GameResources.h"
#include "GameObject.h"

class Tile : public GameObject
{
public:
	Tile(CL_Angle dir, bool solid, CL_Pointf loc, string imageName, string name, string outlineName);
	Tile(CL_Pointf vect, bool solid, CL_Pointf loc, string imageName, string name, string outlineName);
	bool getSolid();
	void setSolid(bool solid);
	void adjustDirection(GameObject *obj);
	CL_Pointf adjustedVect(GameObject *obj);

	bool solid;
};

#endif //#ifndef TILE_INCLUDE