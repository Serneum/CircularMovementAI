#include "../headers/GameResources.h"
#include "../headers/Tile.h"

namespace Resources
{
	bool debug = false;
	//Used for console prints
	char buf[512];
	CL_DisplayWindow* window = NULL;
	CL_ConsoleWindow* console = NULL;
	CL_GraphicContext gc;
	CL_InputContext ic;
	CL_Font_System system_font;
	CL_FontDescription system_font_desc;

	map<string, CL_Sprite> sprites;
	map<string, CL_CollisionOutline> outlines;

	void initResources();
	void dbToggle();

	NNetwork* net;
	
	CL_Sprite getSprite(string name);
	CL_CollisionOutline getOutline(string name);
	
	//As printf, but shoots to the console instead
	void cprintf(char* format, ...);
	//Same, but prints to the given (x,y) position on the window
	void wprintf(char* format, int x, int y, CL_Colorf color, ...);
	void wprintf(char* format, CL_Pointf loc, CL_Colorf color, ...);
}

void Resources::dbToggle()
{
	debug = !debug;
}

void Resources::initResources()
{
	srand(time(NULL));
	console = new CL_ConsoleWindow("Console", 80, 160);
	window = new CL_DisplayWindow("Tactics", SCREEN_WIDTH, SCREEN_HEIGHT);
	gc = window->get_gc();
	ic = window->get_ic();

	// Initialize a system font
	system_font_desc.set_typeface_name("tahoma");
	system_font_desc.set_height(15);
	
	system_font = CL_Font_System(gc, system_font_desc);

	//Create the Neural Network
	net = new NNetwork(2, NUM_OUT, HIDDEN_NODES, LEARN_RATE);
}

void Resources::cprintf(char* format, ...)
{
	va_list args;
	va_start(args, format);
	
	vsprintf(buf, format, args);
	
	CL_Console::write_line(buf);
	
	va_end(args);   
}

void Resources::wprintf(char* format, int x, int y, CL_Colorf color, ...)
{
	va_list args;
	va_start(args, color);
	
	vsprintf(buf, format, args);
	
	system_font.draw_text(gc, x, y, buf, color);
	
	va_end(args);   
}

void Resources::wprintf(char* format, CL_Pointf loc, CL_Colorf color, ...)
{
	va_list args;
	va_start(args, color);
	
	vsprintf(buf, format, args);
	
	system_font.draw_text(gc, loc, buf, color);
	
	va_end(args);   
}

CL_Sprite Resources::getSprite(string name)
{
	CL_Sprite ret;
	map<string, CL_Sprite>::const_iterator tmp = sprites.find(name);

	//not in the map, load it up
	if(tmp == sprites.end())
	{
		CL_SpriteDescription desc = CL_SpriteDescription();
		//TODO In the future when we add multi-frame sprites, we'll need an additional file
		//that lists:
		//1)the number of frames, 2)the names of the files that contain each frame 3)frame durations (optional)
		desc.add_frame(CL_ImageProviderFactory::load("images/" + name + ".png"));
		ret = CL_Sprite(gc, desc);
		ret.set_base_angle(CL_Angle::from_degrees(0));

		sprites[name] = ret;

		ret = CL_Sprite(gc);
		ret.clone(sprites[name]);
	}
	else
	{
		ret = CL_Sprite(gc);
		ret.clone(tmp->second);
	}

	return ret;
}

CL_CollisionOutline Resources::getOutline(string name)
{
	CL_CollisionOutline ret;
	//map<string, CL_CollisionOutline>::const_iterator tmp = outlines.find(name);

	//not in the map, load it up
	//if(tmp == outlines.end())
	{
		ret.load(CL_StringRef("outlines/" + name + ".out"));
		ret.enable_collision_info(true);
		ret.calculate_radius();
		ret.calculate_sub_circles();
		//outlines[name] = ret;

		//ret.copy(outlines[name]);
	}
	//else ret.copy(tmp->second);

	return ret;
}

//Reads a file for a tile-map
vector<vector<Tile*>> Resources::readMap(string fName)
{
	//Create the filename
	string name = "docs/maps/";
	name.append(fName);
	name.append(".map");

	//Open the file
	ifstream in;
	in.open(name.c_str(), ios::in);

	char next;
	int numRows, numColumns;

	//Create and clear the return vector
	vector<vector<Tile*>> ret;
	ret.clear();

	in >> numRows;
	in >> numColumns;
	//For each row...
	for(int i = 0; i < numColumns; i++)
	{
		//Clear the vector
		ret.push_back(vector<Tile*>());
		ret[i].clear();
		//For each column...
		for(int j = 0; j < numRows; j++)
		{
			//Read the next character
			in >> next;
			//Check the character
			switch(next)
			{
			//Insert the blank tile
			case '0':
				 ret[i].push_back(new Tile(CL_Angle::from_degrees(0), false, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.5-block", "empty", "1.5-block"));
				break;
			//South-west-facing wall
			case '1':
				ret[i].push_back(new Tile(CL_Angle::from_degrees(135), true, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.3-block", "South-West", "1.3-block"));
				break;
			//South-facing wall
			case '2':
				ret[i].push_back(new Tile(CL_Angle::from_degrees(90), true, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.0-block", "South", "1.0-block"));
				break;
			//South-east-facing wall
			case '3':
				ret[i].push_back(new Tile(CL_Angle::from_degrees(45), true, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.2-block", "South-East", "1.2-block"));
				break;
			//West-facing wall
			case '4':
				ret[i].push_back(new Tile(CL_Angle::from_degrees(180), true, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.0-block", "West", "1.0-block"));
				break;
			//Neutral wall
			case '5':
				 ret[i].push_back(new Tile(CL_Angle::from_degrees(0), true, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.0-block", "Neutral", "1.0-block"));
				break;
			//East-facing wall
			case '6':
				ret[i].push_back(new Tile(CL_Angle::from_degrees(0), true, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.0-block", "East", "1.0-block"));
				break;
			//North-west-facing wall
			case '7':
				ret[i].push_back(new Tile(CL_Angle::from_degrees(225), true, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.4-block", "North-West", "1.4-block"));
				break;
			//North-facing wall
			case '8':
				ret[i].push_back(new Tile(CL_Angle::from_degrees(270), true, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.0-block", "North", "1.0-block"));
				break;
			//North-east-facing wall
			case '9':
				ret[i].push_back(new Tile(CL_Angle::from_degrees(315), true, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.1-block", "North-East", "1.1-block"));
				break;
			default:
				ret[i].push_back(new Tile(CL_Angle::from_degrees(0), false, CL_Pointf((float)(j*TILE_SIZE + TILE_SIZE/2), (float)(i*TILE_SIZE + TILE_SIZE/2)), "1.5-block", "empty", "1.5-block"));
				break;
			}
		}
	}

	//Return the tile-map
	return ret;
}

//Returns a random value where -1 < val < 1
inline double Resources::RandomClamped()
{
	return (rand())/(RAND_MAX + 1.0) - (rand())/(RAND_MAX + 1.0);
}