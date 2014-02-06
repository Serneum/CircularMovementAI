/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 4/25/2011
* File: Slime.h
* Purpose: Defines a slime and all of the functions
*****************************************************************/

#include "../headers/Slime.h"

Slime::Slime(int xVal, int yVal, double dirVal, int sight, int radar, int faction)
: Agent(100, 1, 10, xVal, yVal, dirVal, sight, 3, radar, 5, 5, "slime", "slime", "slime", faction)
{}