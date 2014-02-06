#ifndef GAME_RESOURCES_INCLUDE
#define GAME_RESOURCES_INCLUDE

#define POINTS_NEEDED 50
#define NUM_CASES 10
#define HIDDEN_NODES 10
#define LEARN_RATE .5
#define NUM_OUT 2

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILE_SIZE 16

#include <ClanLib/gl.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>
#include <ClanLib/Core/Math/angle.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <vector>
#include <time.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include "NNetwork.h"
#include <fstream>

using namespace std;
class Tile;
namespace Resources
{
	extern bool debug;
	//Used for console prints
	extern char buf[];
	extern CL_DisplayWindow* window;
	extern CL_ConsoleWindow* console;
	extern CL_GraphicContext gc;
	extern CL_InputContext ic;
	extern CL_Font_System system_font;
	extern CL_FontDescription system_font_desc;

	extern map<string, CL_Sprite> sprites;
	extern map<string, CL_CollisionOutline> outlines;

	extern NNetwork* net;

	extern void initResources();
	extern void dbToggle();
	
	extern CL_Sprite getSprite(string name);
	extern CL_CollisionOutline getOutline(string name);
	extern vector<vector<Tile*>> readMap(string fName);

	extern inline double RandomClamped();

	//As printf, but shoots to the console instead
	extern void cprintf(char* format, ...);
	//Same, but prints to the given (x,y) position on the window
	extern void wprintf(char* format, int x, int y, CL_Colorf color, ...);
	extern void wprintf(char* format, CL_Pointf loc, CL_Colorf color, ...);
}

#endif //RESOURCES_INCLUDE