/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 4/25/2011
* File: Slime.h
* Purpose: Defines a slime and all of the functions
*****************************************************************/

#ifndef SLIME_INCLUDE
#define SLIME_INCLUDE

#include "Agent.h"

class Slime : public Agent
{
//Create slime specific functions
public:
/*
	Creates a new slime agent that can be drawn to the screen
	Author: Serneum
	Maintainer: Serneum
	Last Updated: 4/25/2011
*/
	Slime(int x, int y, double dir, int sightRange, int radarRange, int faction);
};
#endif //ifndef SLIME_INCLUDE