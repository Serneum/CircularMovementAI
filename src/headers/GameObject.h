/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 2/8/2011
* File: GameObject.h
* Purpose: Defines a generic game object that can be extended
*****************************************************************/

#ifndef GAME_OBJECT_INCLUDE
#define GAME_OBJECT_INCLUDE

#include "../headers/GameResources.h"

class GameObject
{
//Only allow access to "getting" functions and the constructor
public:
	GameObject(int x, int y, string name, string imageName, string outlineName);
	GameObject(CL_Pointf loc, string name, string imageName, string outlineName);

	int getID();
	void setID(int newID);
	double getX();
	double getY();
	CL_Pointf getLoc();
	double getDX();
	void setDX(double newDX);
	double getDY();
	void setDY(double newDY);
	CL_Pointf getVel();
	void setVel(CL_Pointf newVel);
	
	string getName();
	string getSpriteName();
	string getOutlineName();
	CL_Sprite getSprite();
	CL_CollisionOutline getOutline();
	
	bool collides(GameObject* obj);
	bool collides(CL_CollisionOutline cl);
	virtual void update();
	virtual void draw();
	virtual void debugDraw();
	
/*
	Creates a new Agent that can be drawn to the screen
	Author: Zulu
	Maintainer: Zulu
	Last Updated: 3/19/2011
	Zulu:	These should all work now. 
			However, it needs to be known if
			the engine -wants- backwards angles.
*/
	double distanceTo(GameObject* target);
	double distanceToSqrd(GameObject* target);
	CL_Angle angleBetween(GameObject* target);
	
	double distanceTo(CL_Pointf point);
	double distanceToSqrd(CL_Pointf point);
	CL_Angle angleBetween(CL_Pointf point);

	void setX(double newX);
	void setY(double newY);
//Keep all "setting" functions private so that only the engine modifies it
protected:
	int ID;
	CL_Pointf loc;
	CL_Pointf vel;
	CL_Angle angle;
	string name, spriteName, outlineName;
	
	void setLoc(CL_Pointf newLoc);
	CL_Sprite sprite;
	CL_CollisionOutline outline;
	void setName(string name);
	void setSprite(string newSprite);
	void setOutline(string newOutline);
};
#endif //ifndef GAME_OBJECT_INCLUDE