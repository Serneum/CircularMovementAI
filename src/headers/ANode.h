/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 4/20/2011
* File: ANode.h
* Purpose: Defines a node that can be used in A* searching
*****************************************************************/

#include <vector>
using namespace std;

class ANode
{
public:
	vector<ANode*> neighbors;
	double gScore;
	double hScore;
	double fScore;
	ANode* cameFrom;

	ANode(int x, int y, vector<ANode*> others);
	int getX();
	int getY();
private:
	int x;
	int y;
};

//Create the new node
ANode::ANode(int newX, int newY, vector<ANode*> neighb) : gScore(0), hScore(0), fScore(0)
{
	x = newX;
	y = newY;
	neighbors = neighb;
}

//Getter for the node's X value
int ANode::getX()
{
	return x;
}

//Getter for the node's Y value
int ANode::getY()
{
	return y;
}