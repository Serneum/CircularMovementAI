/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 4/25/2011
* File: Square.h
* Purpose: Defines a square and all of the functions
*****************************************************************/

#ifndef SQUARE_INCLUDE
#define SQUARE_INCLUDE

#include "Agent.h"

class Square : public Agent
{
//Create slime specific functions
public:
/*
	Creates a new square agent that can be drawn to the screen
	Author: Serneum
	Maintainer: Serneum
	Last Updated: 4/25/2011
*/
	Square(int x, int y, double dir, int sightRange, int radarRange, int faction);
};
#endif //ifndef SLIME_INCLUDE