/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 4/25/2011
* File: Square.h
* Purpose: Defines a square and all of the functions
*****************************************************************/

#include "../headers/Square.h"

Square::Square(int xVal, int yVal, double dirVal, int sight, int radar, int faction)
: Agent(100, 10, 10, xVal, yVal, dirVal, sight, 3, radar, 5, 5, "square", "square", "square", faction)
{}