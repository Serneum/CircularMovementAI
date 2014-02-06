/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 3/19/2011
* File: DataHandler.h
* Purpose: Defines the object that handles data sets and reads
* and writes to the appropriate files
*****************************************************************/

#ifndef DATA_INCLUDE
#define DATA_INCLUDE

#include "GameResources.h"
#include <ClanLib/core.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "NNetwork.h"
#include <vector>

using namespace std;
using namespace Resources;

class DataSet
{
public:
	DataSet(int feelerLen, float distance, CL_Angle angle);
	string toString();
	bool operator==(const DataSet &ds);
	DataSet &operator+(const DataSet &ds);
private:
	double thrust;
	double turn;
	double dist;
	double angle;
	void setThrust();
	void setTurn();
	void setDist(int feelerLen, float distance);
	void setAngle(CL_Angle angle);
};

namespace DataHandler
{
	extern vector<vector<double>> getInput();
	extern vector<vector<double>> getOutput();
	extern ifstream in;
	extern ofstream out;
	extern vector<double> sensors;
	extern void init();
	//Prints to a file
	extern void fprintf(string s);

	//Reads from the training file
	extern void dataIn();
	extern void convertPoints(vector<DataSet> &in);
	extern vector<vector<double>> inputs;
	extern vector<vector<double>> outputs;
}

#endif