/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 1/24/2011
* File: Agent.h
* Purpose: Defines an agent and all of the functions
*****************************************************************/

#ifndef AGENT_INCLUDE
#define AGENT_INCLUDE

#include "../headers/GameResources.h"
#include "../headers/GameObject.h"
#include "../headers/Tile.h"
#include "../headers/NNetwork.h"

class Agent : public GameObject
{
//Create agent specific functions
public:
	int hp, attack, baseAP, currAP;
	int moveSpeed, rotSpeed;
	int sightRange, radarRange, feelerDist;
	int fitness, faction;
	vector<CL_CollisionOutline> feelers;
	vector<CL_Pointf> feelerCollisionPoints;
	vector<GameObject*> objectsInRadar;
	vector<GameObject*> pieReadings[4];
	double closestInSlice[4];
	NNetwork* net;
/*
	Creates a new Agent that can be drawn to the screen
	Author: Serneum
	Maintainer: Serneum
	Last Updated: 2/12/2011
	Zulu: Fixed syntax from renaming 'initRays' to 'setFeelers'
*/
	Agent(int health, int att, int AP, int x, int y, double dir, int sightRange, int numFeel, int radarRange,
		int movSpeed, int rotSpeed, string name, string spriteName, string outlineName, int faction);

	NNetwork* getNet(void);
	void setNet(NNetwork* net);
/*
	Returns the current direction/facing of the agent
	Author: Serneum
	Maintainer: Serneum
	Last Updated: ??
	Serneum: ??
*/
	double getDir();
/*
	Returns the current direction/facing of the agent
	Author: Zulu
	Maintainer: Zulu
	Last Updated: 2/12/2011
	Zulu: Created
	double getDir();
	int getSightRange();
*/
	int getSightRange();
/*
	Rotate the image and the feelers
	Author: Serneum
	Maintainer: Serneum
	Last Updated: 2/12/2011
	Zulu: Renamed from 'initRays' and fixed syntax errors that came from changing 'feelers' to be a vector
*/
	void rotate(double change);

	int getRadarRange();

/*
	Moves the agent along its current heading (+ is forwards, - is backwards)
	Author: Zulu
	Maintainer: Zulu
	Last Updated: 2/13/2011
*/
	void moveDir(double change);

/*
	Takes care of anything that needs to be done when the position (x, y) is changed
	Author: Zulu
	Maintainer: Zulu
	Last Updated: 2/13/2011
*/
	void updatePos();

/*	
	Runs the various sensors through the elements of both lists, accumulating data
	Author: Zulu
	Maintainer: Zulu
	Last Updated: 2/13/2011
	Zulu: Created
*/
	void sense(const vector<Agent*> &agents, const vector<vector<Tile*>> &tiles);
/*	
	Returns a vector which represents the shortest distance to interception 
	for each feeler when using the provided target list
	Author: Zulu
	Maintainer: Zulu
	Last Updated: 2/13/2011
	Zulu: Now also saves the data into the feelerCollisions vector
*/
	void feelerSense(const vector<vector<Tile*>> &tiles);
	
/*
	Adjacent agent sensor.  Returns a list all agents within a fixed radius (the 
	sensor’s range) of the agent.  Each entry in the list should identify the 
	agent, and specify its distance from the subject and its relative heading,
	i.e. the angle from the subject’s heading to the detected agent (you do not 
	need to take into account the detected agent’s heading)
*/
	void radarSense(const vector<Agent*> &agentList);
	
	void pieSense();

/*
	Seek function. Used to pinpoint a target's location and then go towards it. For now
	it is designed to go to an object with an unobstructed path, but it can be adapted to use
	some pathfinding techniques to avoid any obstacles.
	Author: Serneum
	Maintainer: Serneum
	Last Updated: 3/19/2011
*/
	void seek(GameObject* o);

	void draw();

	void debugDraw();

/*
	Ends an agent's turn in the game. Adds additional action points that will be reusable in the
	next turn.
	Author: Serneum
	Maintainer: Serneum
	Last Updated: 4/22/2011
*/
	void endTurn();

/*
	Updates the current AP of the agent. Specifically coded to deal with movement where movement
	will take up to 90% of the agent's base AP based on how much of the max distance the agent 
	travelled during the turn
	Author: Serneum
	Maintainer: Serneum
	Last Updated: 4/22/2011
*/
	void updateAP(double distance);

/*
	Takes the current sensor information as well as the current AP of the agent and normalizes
	the data. The data is then passed to the individual neural networks associated with each
	agent and the responses from the networks are used to tell the agents how to behave
	Author: Serneum
	Maintainer: Serneum
	Last Updated: 4/22/2011
*/
	void update();

protected:
/*
	Sets the feelers of this object to be the given ammount and length.
	If an even ammount is given, the ammount is increaded by one.
	Author: Serneum
	Maintainer: Serneum
	Last Updated: 2/12/2011
	Zulu: Renamed from 'initRays' and fixed syntax errors that came from changing 'feelers' to be a vector
*/
	void setFeelers(int count, int length);
/*
	Updates the sight range for the object and re-sets feelers to match
	Author: Zulu
	Maintainer: Zulu
	Last Updated: 2/12/2011
	Zulu: Created
*/
	void setSightRange(int range);
	void setRadarRange(int range);
/*
	Updates the current direction/facing of the agent if it has been changed
	Author: Serneum
	Maintainer: Serneum
	Last Updated: ??
	Serneum: ??
*/
	void updateDir(double change);
};
#endif //ifndef AGENT_INCLUDE