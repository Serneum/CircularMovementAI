/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 2/8/2011
* File: GameObject.cpp
* Purpose: Create the base GameObject that can be extended as 
* needed. Holds position and the collision outline
*****************************************************************/
#include "../headers/GameObject.h"

GameObject::GameObject(int x, int y, string name, string spriteName, string outlineName)
{
	setVel(CL_Pointf(0, 0));
	setX((double)x);
	setY((double)y);
	setName(name);
	setSprite(spriteName);
	setOutline(outlineName);
}

GameObject::GameObject(CL_Pointf loc, string name, string spriteName, string outlineName)
{
	setVel(CL_Pointf(0, 0));
	setLoc(loc);
	setName(name);
	setSprite(spriteName);
	setOutline(outlineName);
}

int GameObject::getID()
{
	return ID;
}

void GameObject::setID(int newID)
{
	ID = newID;
}

//Returns the X coordinate of the agent
double GameObject::getX()
{
	return this->loc.x;
}

//Sets the X coordinate of the agent
void GameObject::setX(double newX)
{
	this->loc.x = (float)newX;
}

//Returns the Y coordinate of the agent
double GameObject::getY()
{
	return this->loc.y;
}

//Sets the Y coordinate of the agent
void GameObject::setY(double newY)
{
	this->loc.y = (float)newY;
}

//Returns the location of the agent (x,y)
CL_Pointf GameObject::getLoc()
{
	return this->loc;
}

//Returns the location of the agent (x,y)
void GameObject::setLoc(CL_Pointf newLoc)
{
	this->loc = newLoc;
}

double GameObject::getDX()
{
	return vel.x;
}

void GameObject::setDX(double newDX)
{
	vel.x = (float)newDX;
}

double GameObject::getDY()
{
	return vel.y;
}

void GameObject::setDY(double newDY)
{
	vel.y = (float)newDY;
}

CL_Pointf GameObject::getVel()
{
	return vel;
}

void GameObject::setVel(CL_Pointf newVel)
{
	vel = newVel;
}

//Sets the "name" of the object
void GameObject::setName(string newName)
{
	name = newName;
}

//Gets the name of the object
string GameObject::getName()
{
	return this->name;
}

//Grabs the image name if it's ever needed
string GameObject::getSpriteName()
{
	return spriteName;
}

CL_Sprite GameObject::getSprite()
{
	return sprite;
}

void GameObject::setSprite(string newSprite)
{
	spriteName = newSprite;
	sprite = Resources::getSprite(newSprite);
	sprite.set_alignment(CL_Origin::origin_center, 0, 0);
}

//Loads the collision outline
CL_CollisionOutline GameObject::getOutline()
{
	return outline;
}

//Sets the collision outline of this object
void GameObject::setOutline(string name)
{
	outlineName = name;
	outline = Resources::getOutline(name);
	
	//Center on (x,y)
	outline.set_translation((float)getX() - outline.get_width()/2, (float)getY() - outline.get_height()/2);
	outline.set_rotation_hotspot(CL_Origin::origin_center, 0, 0);
}

//Returns true if the object collides with another object
bool GameObject::collides(GameObject* obj)
{
	return collides(obj->getOutline());
}

//Returns true if the object collides with the given collision outline
bool GameObject::collides(CL_CollisionOutline cl)
{
	return this->getOutline().collide(cl, true);
}

void GameObject::update()
{
	sprite.update();
}

void GameObject::draw()
{
	sprite.draw(Resources::gc, (float)getX(), (float)getY());
}

void GameObject::debugDraw()
{
	outline.draw(0, 0, CL_Colorf::yellow, Resources::gc);
}

double GameObject::distanceTo(GameObject* target)
{
	return sqrt(distanceToSqrd(target));
}

double GameObject::distanceToSqrd(GameObject* target)
{
	return (target->getX() - getX())*(target->getX() - getX()) + (target->getY() - getY())*(target->getY() - getY());
}

CL_Angle GameObject::angleBetween(GameObject* target)
{
	CL_Angle ret = CL_Angle::from_radians((float)atan2(target->getY() - getY(), target->getX() - getX()));
	ret -= (angle);
	ret.normalize();

	return ret;
}

double GameObject::distanceTo(CL_Pointf point)
{
	return sqrt(distanceToSqrd(point));
}

double GameObject::distanceToSqrd(CL_Pointf point)
{
	return (point.x - getX())*(point.x - getX()) + (point.y - getY())*(point.y - getY());
}

CL_Angle GameObject::angleBetween(CL_Pointf point)
{
	CL_Angle ret = CL_Angle::from_radians((float)atan2(getY() - point.y, getX() - point.x));
	
	ret -= (angle);
	ret += CL_Angle::from_degrees(180);
	ret.normalize();

	return ret;
}