/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 1/24/2011
* File: Agent.cpp
* Purpose: Define an agent with an angle and position. Can be 
* converted to player/creature later.
*****************************************************************/
#include "../headers/Agent.h"
#include <sstream>

Agent::Agent(int health, int att, int AP, int x, int y, double dir, int sightRange, int numFeel,
	int radarRange, int mSpeed, int rSpeed, string name, string spriteName, string outlineName, int fact)
	: GameObject(x, y, name, spriteName, outlineName)
{
	//Set the gameplay variables
	baseAP = currAP = AP;
	hp = health;
	attack = att;

	//Set the movement variables
	moveSpeed = mSpeed;
	rotSpeed = rSpeed;

	//Set the faction
	faction = fact;

	//Set the starting angle to "0"
	angle.set_degrees(0);

	//Make sure there's always an odd number of feelers
	if(!(numFeel%2))
		numFeel++;
	setFeelers(numFeel, sightRange);

	//Set up any sensors
	setSightRange(sightRange);
	setRadarRange(radarRange);

	//Set up the pie sensor to accept a vector of game objects
	for(unsigned i = 0; i < 4; i++)
		pieReadings[i] = vector<GameObject*>();

	rotate(360 - dir);
	net = NULL;
	fitness = 0;
}

NNetwork* Agent::getNet()
{
	return net;
}

void Agent::setNet(NNetwork* net)
{
	this->net = net;
}

void Agent::setFeelers(int count, int length)
{
	//Make sure there's always an odd number of rays
	if(!(count%2))
		count++;

	//Allocate memory for the array of distances for each ray
	feelers = vector<CL_CollisionOutline>(count);
	feelerCollisionPoints = vector<CL_Pointf>(count);

	//Determine how many degrees between each ray
	double deg = (double)180/count;
	feelerDist = (int)deg;

	//Create a temporary angle that can be used to modify ray position
	CL_Angle tmp;
	
	for(int i = 0; i < count; i++)
	{
		//Create a new ray and enable the collision info
		feelers[i] = CL_CollisionOutline();
		feelers[i].set_inside_test(false);
		feelers[i].enable_collision_info(true);
		

		//Change the angle of the rays as they get further from the center
		if(i <= count/2)
		{
			tmp.set_degrees((float)(getDir() - deg * i));
			feelers[i].set_angle(tmp.normalize());
		}
		else
		{
			tmp.set_degrees((float)(getDir() + deg * (i - count/2)));
			feelers[i].set_angle(tmp.normalize());
		}
		
		//Create a contour and add start/end points for each ray/feeler
		CL_Contour contour;
		
		contour.get_points().push_back(CL_Pointf(0, 0));
		contour.get_points().push_back(CL_Pointf(cos(tmp.to_radians()) * length, sin(-1 * tmp.to_radians()) * length));

		//Push the contour onto the collision outline and set any collision info
		feelers[i].get_contours().push_back(contour);
		
		feelers[i].calculate_radius();
		feelers[i].calculate_smallest_enclosing_discs();
		feelers[i].set_translation((float)getX(), (float)getY());

		feelerCollisionPoints[i] = CL_Pointf(feelers[i].get_contours().back().get_points().back());
	}
}

void Agent::seek(GameObject* o)
{
	unsigned int i = 0, j = 0, index = 99;
	double dist = distanceTo(o);
	CL_Angle angle = angleBetween(o);
	
	//Loop through each of the pie slices. If a slice has objects, look for 
	//the target. If it's there, grab the pie slice it's in
	for(i = 0; i < 4; i++)
		if(pieReadings[i].size() != 0)
			for(j = 0; j < pieReadings[i].size(); j++)
				if(pieReadings[i].at(j) == o)
					index = i;

	if(angle.to_degrees() - 360  < -358 && angle.to_degrees() < 1 && dist > 2)
		moveDir(2);
	//Use the index to determine which side the target is on and rotate accordingly.
	else if(index == 0)
	{
		//If it's on the left side of "right in front", then rotate left
		if(angle.to_degrees() > 310)
			rotate(-1);
		else
			rotate(1);
	}
	//If it's to the right/behind, rotate right
	else if(index == 1)
		rotate(1);
	else if(index == 2)
	{
		//If it's on the right side of "behind you", then rotate right
		if(angle.to_degrees() < 180)
			rotate(1);
		else
			rotate(-1);
	}
	//If it's to the left, rotate left
	else //if(index == 4)
		rotate(-1);

	//DEBUG PRINT
	//Resources::wprintf("Angle - 360: %lf, Angle: %lf, Distance: %lf", 50, 50, CL_Colorf::white, angle.to_degrees() - 360, angle.to_degrees(), dist);
}

void Agent::rotate(double change)
{
	//Rotate all of the feelers
	for(unsigned int i = 0; i < feelers.size(); i++)
		feelers[i].rotate(CL_Angle::from_degrees((float)change));

	//Rotate and update the agent
	updateDir(change);
}

int Agent::getSightRange()
{
	//Get the sight range of the agent
	return sightRange;
}

void Agent::setSightRange(int range)
{
	//Set the new sight range of the agent and update the feelers
	sightRange = range;
	setFeelers(feelers.size(), range);
}

int Agent::getRadarRange()
{
	//Get the radar range of the agent
	return radarRange;
}

void Agent::setRadarRange(int range)
{
	//Set the new radar range of the agent
	radarRange = range;
}

double Agent::getDir()
{
	//Get the direction that the agent is facing
	return angle.to_degrees();
}

void Agent::moveDir(double change)
{
	//Get the change in position of the agent
	double deltaX = cos(angle.to_radians()) * change;
	double deltaY = sin(angle.to_radians()) * change;

	//Set the new values of X/Y
	setDX(getDX() + deltaX);
	setDY(getDY() + deltaY);
}

void Agent::updateDir(double change)
{
	CL_Angle tmp = CL_Angle::from_degrees((float)change);

	//Update the angle of the agent and normalize it
	angle += tmp;
	angle.normalize();
	//Rotate the collision outline and the sprite
	outline.rotate(tmp);
	sprite.rotate(tmp);
}

void Agent::updatePos()
{
	setX(getX() + getDX());
	setY(getY() + getDY());
	setVel(CL_Pointf(0, 0));
	//Update positions of the feelers
	for(unsigned int i = 0; i < feelers.size(); i++)
		feelers[i].set_translation((float)getX(), (float)getY());
	//Update position of the collision outline
	outline.set_translation((float)(getX() - outline.get_width()/2), (float)(getY() - outline.get_height()/2));
}

void Agent::sense(const vector<Agent*> &agents, const vector<vector<Tile*>> &tiles)
{
	feelerSense(tiles);
	radarSense(agents);
	pieSense();
}

void Agent::feelerSense(const vector<vector<Tile*>> &tiles)
{
	unsigned int i, j, k, l, m;
	vector<double> tmp = vector<double>(feelers.size());
	Tile* curr;

	//Initialize each distance value to the max sight range (squared for efficiency purposes)
	for(i = 0; i < tmp.size(); i++)
	{
		tmp[i] = (double)(sightRange*sightRange);

		feelerCollisionPoints[i] = feelers[i].get_contours().back().get_points().back();
	}

	//for each target
	for(i = 0; i < tiles.size(); i++)
	{
		for(j = 0; j < tiles[i].size(); j++)
		{
			curr = tiles[i][j];

			if(!curr->getSolid())
				continue;

			//for each feeler
			for(k = 0; k < tmp.size(); k++)
			{
				//if the feeler and target don't collide, skip
				//Skip allies
				if(!curr->collides(feelers[k]))
					continue;
				
				//else if they collide, find out where and see if it's closer for this ray than a previous result
				const vector<CL_CollidingContours>& colpointinfo = curr->getOutline().get_collision_info();
				
				// Loop through all pairs of colliding contours
				for(l = 0; l < colpointinfo.size(); l++)
				{
					//Pretty sure these two loops are uneccesary, as feelers are a single contour
					const vector<CL_CollisionPoint> cc = colpointinfo[l].points;
					for(m = 0; m < cc.size(); m++)
					{
						const double distSqrd = distanceToSqrd(cc[m].point);

						if(distSqrd < tmp[k])
						{
							//Save closest distance for this feeler
							tmp[k] = distSqrd;
							//Save closest point for this feeler
							feelerCollisionPoints[k] = cc[m].point;
						}
					}
				}
			}
		}
	}
}

void Agent::radarSense(const vector<Agent*> &targetList)
{
	objectsInRadar.clear();
	string spriteName = getSpriteName();

	for(unsigned int i = 0; i < targetList.size(); i++)
	{
		//skip yourself and all allies
		if(!spriteName.compare(targetList[i]->getSpriteName()))
			continue;

		if(distanceToSqrd(targetList[i]) <= radarRange*radarRange)
			objectsInRadar.push_back(targetList[i]);
	}
}

void Agent::pieSense()
{
	unsigned int i;
	CL_Angle tmp;
	double dist;

	//Clear any old data
	for(i = 0; i < 4; i++)
	{
		pieReadings[i].clear();
		closestInSlice[i] = radarRange * radarRange;
	}

	for(i = 0; i < objectsInRadar.size(); i++)
	{
		//Grab the angle between the agent and the object
		tmp = angleBetween(objectsInRadar[i]);
		//Get the "distance" to the object
		dist = distanceToSqrd(objectsInRadar[i]);
		
		//For each of the conditions below, the object in the radar is checked and placed
		//in the appropriate pie slice. Each object is also checked to see if it is closer
		//to the agent than the current closest object in that pie slice
		if((45 >= tmp.to_degrees() && tmp.to_degrees() >= 0) || (tmp.to_degrees() > 315))
		{
			pieReadings[0].push_back(objectsInRadar[i]);

			if(pieReadings[0].size() == 1 || dist <= closestInSlice[0])
				closestInSlice[0] = dist;
		}
		else if(135 >= tmp.to_degrees() && tmp.to_degrees() > 45)
		{
			pieReadings[1].push_back(objectsInRadar[i]);

			if(pieReadings[1].size() == 1 || dist <= closestInSlice[1])
				closestInSlice[1] = dist;
		}
		else if(225 >= tmp.to_degrees() && tmp.to_degrees() > 135)
		{
			pieReadings[2].push_back(objectsInRadar[i]);

			if(pieReadings[2].size() == 1 || dist <= closestInSlice[2])
				closestInSlice[2] = dist;
		}
		else //if(315 >= tmp.to_degrees() && tmp.to_degrees() > 225)
		{
			pieReadings[3].push_back(objectsInRadar[i]);

			if(pieReadings[3].size() == 1 || dist <= closestInSlice[3])
				closestInSlice[3] = dist;
		}
	}

	//Get actual distances to the closest objects
	for(int i = 0; i < 4; i++)
		closestInSlice[i] = sqrt(closestInSlice[i]);
}

void Agent::draw()
{
	GameObject::draw();
}

void Agent::debugDraw()
{
	GameObject::debugDraw();

	unsigned int i;
	//Feelers
	for(i = 0; i < feelers.size(); i++)
	{
		Resources::wprintf("%.1f", feelerCollisionPoints[i], CL_Colorf::yellow, distanceTo(feelerCollisionPoints[i]));
		CL_Draw::line(Resources::gc, getLoc(), feelerCollisionPoints[i], CL_Colorf::blue);
	}

	//Radar
	//CL_Draw::circle(Resources::graphicContext, CL_Pointf(x, y), radarRange, Resources::radarColor);
	for(i = 0; i < objectsInRadar.size(); i++)
	{
		Resources::wprintf("%.1f", objectsInRadar[i]->getLoc(), CL_Colorf::red, distanceTo(objectsInRadar[i]));
		CL_Draw::line(Resources::gc, getLoc(), objectsInRadar[i]->getLoc(), CL_Colorf::red);
	}

	//Commented out while I finish making the transition
	
	//Pie slices
	CL_Draw::line(Resources::gc,
		CL_Pointf(cos(CL_Angle::from_degrees(225 + getDir()).to_radians())*getRadarRange() + getX(), sin(CL_Angle::from_degrees(225 + getDir()).to_radians())*getRadarRange() + getY()),
		CL_Pointf(cos(CL_Angle::from_degrees(45 + getDir()).to_radians())*getRadarRange() + getX(), sin(CL_Angle::from_degrees(45 + getDir()).to_radians())*getRadarRange() + getY()), 
		CL_Colorf::green);

	CL_Draw::line(Resources::gc,
		CL_Pointf(cos(CL_Angle::from_degrees(135 + getDir()).to_radians())*getRadarRange() + getX(), sin(CL_Angle::from_degrees(135 + getDir()).to_radians())*getRadarRange() + getY()),
		CL_Pointf(cos(CL_Angle::from_degrees(315 + getDir()).to_radians())*getRadarRange() + getX(), sin(CL_Angle::from_degrees(315 + getDir()).to_radians())*getRadarRange() + getY()), 
		CL_Colorf::green);
	
	CL_Colorf color;
	color = pieReadings[0].size() ? CL_Colorf::lightgreen:CL_Colorf::white;
	Resources::wprintf("|%d|", (int)(getX() + cos(getDir()*0.0174532925)*getRadarRange()/2), (int)(getY() + sin(getDir()*0.0174532925)*getRadarRange()/2), color, pieReadings[0].size());
	
	color = pieReadings[1].size() ? CL_Colorf::lightgreen:CL_Colorf::white;
	Resources::wprintf("|%d|", (int)(getX() +  cos((getDir() + 90)*0.0174532925)*getRadarRange()/2), (int)(getY() +  sin((getDir() + 90)*0.0174532925)*getRadarRange()/2), color, pieReadings[1].size());

	color = pieReadings[2].size() ? CL_Colorf::lightgreen:CL_Colorf::white;
	Resources::wprintf("|%d|", (int)(getX() + cos((getDir() + 180)*0.0174532925)*getRadarRange()/2), (int)(getY() + sin((getDir() + 180)*0.0174532925)*getRadarRange()/2), color, pieReadings[2].size());

	color = pieReadings[3].size() ? CL_Colorf::lightgreen:CL_Colorf::white;
	Resources::wprintf("|%d|", (int)(getX() +  cos((getDir() + 270)*0.0174532925)*getRadarRange()/2), (int)(getY() +  sin((getDir() + 270)*0.0174532925)*getRadarRange()/2), color, pieReadings[3].size());
}

void Agent::endTurn()
{
	//Add baseAP to the agent's AP pool
	currAP += baseAP;

	//Cap at 200% AP
	if(currAP > (baseAP * 2))
		currAP = baseAP * 2;
}

void Agent::updateAP(double dist)
{

	//Uses up AP based on distance travelled. Max is 90% of baseAP
	currAP -= dist/radarRange * (.9 * baseAP);
}

void Agent::update()
{
	vector<double> in = vector<double>();
	vector<double> response;
	int totalSize = objectsInRadar.size(), i;

	//Push the distance to any colliding object
	for(i = 0; i < feelers.size(); i++)
		in.push_back(distanceTo(feelerCollisionPoints[i])/feelerDist);
	//Push how many enemies are in each pie slice
	for(i = 0; i < 4; i++)
		in.push_back(pieReadings[i].size()/totalSize);
	//Push the distance to the closest object in each pie slice
	for(i = 0; i < 4; i++)
		in.push_back(closestInSlice[i]/radarRange);
	//Push the current amount of AP
	in.push_back((double)currAP/baseAP);

	//Update the network
	response = net->update(in);
	//Move the agent accordingly
	moveDir(response[0] * moveSpeed);
	rotate(response[1] * rotSpeed);
}