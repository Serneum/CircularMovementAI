/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 4/22/2011
* File: Tile.cpp
* Purpose: Creates a tile for use in tile maps
*****************************************************************/

#include "../headers/Tile.h"

Tile::Tile(CL_Pointf vect, bool solid, CL_Pointf loc, string imageName, string name, string outlineName)
: GameObject(loc, name, imageName, outlineName)
{
	setSolid(solid);
	if(solid)
		setVel(vect);
	else
		setVel(CL_Pointf(0, 0));
}

Tile::Tile(CL_Angle dir, bool solid, CL_Pointf loc, string imageName, string name, string outlineName)
: GameObject(loc, name, imageName, outlineName)
{
	CL_Pointf vect = CL_Pointf(cos(dir.to_radians()), sin(dir.to_radians()));

	setSolid(solid);
	if(solid)
		setVel(vect);
	else
		setVel(CL_Pointf(0, 0));
}

void Tile::adjustDirection (GameObject *obj)
{
	CL_Pointf tmp = adjustedVect(obj);
	obj->setDX(vel.x + tmp.x);
	obj->setDY(vel.y + tmp.y);
}

CL_Pointf Tile::adjustedVect(GameObject *obj)
{
	CL_Pointf tmp = obj->getVel();
	tmp.normalize();
	tmp += getVel();
	tmp.normalize();

	return tmp;
}

bool Tile::getSolid()
{
	return solid;
}

void Tile::setSolid(bool solid)
{
	this->solid = solid;
}