/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 1/24/2011
* File: Core.cpp
* Purpose: Create the core of the game including the game loop
*****************************************************************/

#include "../headers/GameResources.h"
#include "../headers/Square.h"
#include "../headers/Slime.h"
#include "../headers/NNetwork.h"
#include "../headers/Tile.h"
#include "../headers/RandomList.h"

#define DELAY_LENGTH 5000

using namespace Resources;
using namespace std;

int delayTime = DELAY_LENGTH;
Square* player;
vector<Agent*> agents;
vector<vector<Tile*>> tileMap;
map<int, void (*)(int)> keyBindings;
CL_Colorf purpzzz((float)272.0, (float)0.0, (float)272.0, 0.25);

void correctCollisions(GameObject* obj);
void gameTick();
void handle_input(CL_InputContext &ic);
void move(int key);
void rotate(int key);
void pause(int key);
void delay();
void run();

//Crossover for genomes
vector<NNetwork*> cross(vector<Agent*> heap);

void gameTick()
{
	//unsigned int i, j, k;
}

#define TILE_SIZE 32

void correctCollisions(GameObject* obj)
{
	unsigned i, j;
	Tile* currTile = tileMap[obj->getY()/TILE_SIZE][obj->getX()/TILE_SIZE];

	//Very unoptimized way of detecting collisions
	for(i = 0; i < tileMap.size(); i++)
		for(j = 0; j < tileMap[i].size(); j++)
			if(tileMap[i][j]->getSolid() &&
				tileMap[i][j]->collides(obj))
				tileMap[i][j]->adjustDirection(obj);
}

void init()
{
	agents = vector<Agent*>();
	tileMap.clear();
	keyBindings.clear();

	keyBindings[CL_KEY_W] = &move;
	keyBindings[CL_KEY_A] = &rotate;
	keyBindings[CL_KEY_S] = &move;
	keyBindings[CL_KEY_D] = &rotate;
	keyBindings[CL_KEY_P] = &pause;
}

void run()
{
	unsigned int i, j;
	CL_Circle circle;

	Resources::initResources();
	init();

	//Read in the tile map
	tileMap = Resources::readMap("map1");

	player = new Square(400, 400, 90, 100, 100, 0);
	agents.push_back(player);
	agents.push_back(new Square(300, 450, 0, 100, 100, 0));
	agents.push_back(new Square(500, 450, 0, 100, 100, 0));
	agents.push_back(new Slime(225, 210, 0, 100, 100, 1));
	agents.push_back(new Slime(420, 450, 0, 100, 100, 1));
	agents.push_back(new Slime(470, 415, 0, 100, 100, 1));
	

	circle.position = player->getLoc();
	circle.radius = player->getRadarRange();

	while(!ic.get_keyboard().get_keycode(CL_KEY_ESCAPE))
	{
		delayTime++;

		//Draw the background
		CL_Draw::fill(gc, CL_Rectf(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), CL_Colorf::green);

		CL_Draw::circle(gc, player->getX(), player->getY(), 100, purpzzz);

		handle_input(Resources::ic);
		circle.position = player->getLoc();

		//Perform object updates
		for(i = 0; i < agents.size(); i++)
		{
			correctCollisions(agents[i]);
			agents[i]->updatePos();
			agents[i]->draw();
		}

		//Sense and draw agents
		for(i = 0; i < agents.size(); i++)
			agents[i]->sense(agents, tileMap);

		//Draw all of the tiles
		for(i = 0; i < tileMap.size(); i++)
			for(j = 0; j < tileMap[i].size(); j++)
				tileMap[i][j]->draw();
		
		if(Resources::debug)
			//If in debug mode, super-impose the outlines on to the game
			for(i = 0; i < agents.size(); i++)
				agents[i]->debugDraw();
		
		window->flip();

		CL_KeepAlive::process();
		CL_System::sleep(10);
	}
}

int main(const std::vector<CL_String> &args)
{
	//Setup the window basics with openGL
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	
	try
	{
		run();
	}
	catch(CL_Exception &exception)
	{
		Resources::console->wait_for_key();
		CL_Console::write_line("Error: " + exception.get_message_and_stack_trace());
		Resources::console->display_close_message();

		return -1;
	}

	return 0;
}

//Handles all user input through the keyboard
void handle_input(CL_InputContext &ic)
{
	for (std::map<int, void(*)(int)>::iterator it = keyBindings.begin(); it != keyBindings.end(); ++it)
	{
		if(ic.get_keyboard().get_keycode(it->first))
			(*it->second)(it->first);
	}
	
	if(ic.get_keyboard().get_keycode(CL_KEY_V))
		dbToggle();
}

void rotate(int key)
{
	if(key == CL_KEY_A)
		player->rotate(-1*player->rotSpeed);
	else
		player->rotate(player->rotSpeed);
}

void move(int key)
{
	if(key == CL_KEY_S)
		player->moveDir(-1*player->moveSpeed);
	else
		player->moveDir(player->moveSpeed);
}

void pause(int key)
{
	Resources::cprintf("%d", delayTime);
	delay();
	while(!ic.get_keyboard().get_keycode(CL_KEY_P));
}

void delay()
{
	while(1)
	{
		if(delayTime >= DELAY_LENGTH)
		{
			delayTime = 1;
			return;
		}
		else
			delayTime++;
	}
}

void addKeyBinding(int key, void (*fun)(int))
{
	keyBindings[key] = fun;
}

//Calls the program to run the window
CL_ClanApplication app(&main);

vector<NNetwork*> cross(RandomList<Agent*> top)
{
	vector<NNetwork*> ret = vector<NNetwork*>();
	unsigned int i, j, k, l, m;

	//Break into pairs. If the list isn't empty, there are at least two elements inside
	while(!top.isEmpty())
	{
		//Grab two "parents" from the randomized list
		Agent* mom = top.get();
		Agent* dad = top.get();
		//Create 4 offspring for every set of parents
		for(j = 0; j < 4; j++)
		{
			ret.push_back(new NNetwork());
			NNetwork tmp, tmp2;

			//Grab the networks from the parents
			tmp = *(mom->getNet());
			tmp2 = *(dad->getNet());

			//For each layer...
			for(k = 0; k < tmp.layerList.size(); k++)
			{
				//Create a new layer based on the information provided by the parents
				ret[ret.size() - 1]->layerList.push_back(NLayer(tmp.layerList[k].numNodes, tmp.numIn));
				//For each node...
				for(l = 0; l < tmp.layerList[k].numNodes; l++)
				{
					//Create a new node based on the information provided by the parents
					ret[ret.size() - 1]->layerList[k].nodeList.push_back(NNode(tmp.layerList[k].nodeList[l].numIn));
					//For each weight
					for(m = 0; m < tmp.layerList[k].nodeList[l].inWeights.size(); m++)
					{
						//Create a new weight based on the parents
						ret[ret.size() - 1]->layerList[k].nodeList[l].inWeights.push_back(tmp.layerList[k].nodeList[l].inWeights[m]);
						//Crossover from the "dad" if a criteria is met
						if(Resources::RandomClamped() > 0)
							ret[ret.size() - 1]->layerList[k].nodeList[l].inWeights[m] = tmp2.layerList[k].nodeList[l].inWeights[m];
					}
				}
			}
		}
	}
	//Return the list of super-children
	return ret;
}