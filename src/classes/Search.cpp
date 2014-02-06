/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 4/20/2011
* File: Search.cpp
* Purpose: Runs A* on a network of nodes
*****************************************************************/

#include "../headers/ANode.h"
#include <queue>
#include <math.h>

//GLOBAL
vector<ANode*> path = vector<ANode*>();

//Find Euclidean distance between the nodes
int dist(ANode* x, ANode* y)
{
	//Efficiency hack
	return (x->getX() + y->getX()) * (x->getX() + y->getX()) + (x->getY() + y->getY()) * (x->getY() + y->getY());
}

//Return the Euclidean distance between the nodes
int heurScore(ANode* x, ANode* y)
{
	return dist(x, y);
}

//Reconstruct the path
void buildPath(vector<ANode*> visited, ANode* node)
{
	//If we're not at the start, recurse down
	if(node->cameFrom != node)
	{
		path.push_back(node);
		buildPath(visited, node->cameFrom);
	}
}

bool inSet(ANode* node, vector<ANode*> set)
{
	//Check if the node exists in the set
	for(int i = 0; i < set.size(); i++)
		if(node == set[i])
			return true;
	//If it reaches this point, the node is not in the set
	return false;
}

vector<ANode*> AStar(ANode* start, ANode* goal)
{
	//Create the empty vectors that represent sets/maps
	vector<ANode*> closed = vector<ANode*>();
	priority_queue<ANode*> open = priority_queue<ANode*>();
	vector<ANode*> visited = vector<ANode*>();

	//Croeate the first node and push it on to the open set
	start->cameFrom = start;
	start->gScore = 0;
	start->hScore = heurScore(start, goal);
	start->fScore = start->hScore;
	open.push(start);
	visited.push_back(start);

	//Create the boolean to check if scores are better
	bool tmpScoreIsBetter = false;

	//Loop while there are still nodes in the open set
	while(!open.empty())
	{
		//Grab the first node
		ANode* tmp = open.top();
		open.pop();

		//If the node with the lowest score is the goal, build the path to follow
		if(tmp == goal)
		{
			buildPath(visited, goal->cameFrom);
			return path;
		}

		//Push the node on to the closed set
		closed.push_back(tmp);

		//Grab all of the neighboring nodes from the cheapest node
		for(int i = 0; i < tmp->neighbors.size(); i++)
		{
			//Grab the node and check if you will be going to it
			ANode* nextNode = tmp->neighbors[i];
			//If the node has already been seen, skip it
			if(inSet(nextNode, closed))
				continue;
			//Create a temporary "tentative" score
			int tmpG = tmp->gScore + dist(tmp, nextNode);

			//Add it to the open set and the tmpScore is always better
			if(!inSet(nextNode, visited))
			{
				open.push(nextNode);
				visited.push_back(nextNode);
				tmpScoreIsBetter = true;
			}
			//If it's in the open set, see if the current tmp is better
			else
				tmpScoreIsBetter = tmpG < nextNode->gScore;

			//If the tmpScore is better, add it to the list
			if(tmpScoreIsBetter)
			{
				nextNode->cameFrom = tmp;
				nextNode->gScore = tmpG;
				nextNode->hScore = heurScore(nextNode, goal);
				nextNode->fScore = nextNode->gScore + nextNode->hScore;
			}
		}
	}

	//Failed Search. Path will be empty
	return path;
}