/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 3/19/2011
* File: DataHandler.cpp
* Purpose: Handles all reading in/printing out of test cases for
* training the neural network when using backpropagation.
*****************************************************************/

#include "../headers/DataHandler.h"		

namespace DataHandler
{
	ifstream in;
	ofstream out;
	ostringstream wString;
	vector<double> sensors;
	vector<vector<double>> getInput();
	vector<vector<double>> getOutput();

	void init();
	//Prints to a file
	void fprintf(string s);
	void dataIn();
	void convertPoints(vector<DataSet> &in);
	vector<vector<double>> inputs;
	vector<vector<double>> outputs;
}

void DataHandler::init()
{
	in.open("docs/training.txt", ios::in);
	out.open("docs/training.txt", ios::out | ios::app);

	dataIn();
}

void DataHandler::convertPoints(vector<DataSet> &in)
{
	int index = 0;
	unsigned int i = 0;

	while(in.size() > POINTS_NEEDED)
	{
		index = 0;
		for(i = 1; i < in.size(); i++)
		{
			if(in[i] == in[i - 1])
			{
				index = i;
				break;
			}
		}
		
		if(index != 0)
		{
			in[index - 1] = in[index - 1] + in[index];
			in.erase(in.begin() + index);
		}
		else
			//Remove from the middle. Start/end are the most important parts
			in.erase(in.begin() + in.size()/2);
	}
}

void DataHandler::fprintf(string s)
{
	out << s << endl;
}

void DataHandler::dataIn()
{
	double num;

	//Push vectors initialized to 0 into the input list
	for(unsigned int i = 0; i < NUM_CASES; i++)
	{
		inputs.push_back(vector<double>());
		outputs.push_back(vector<double>());
	}

	//Read values into the vectors
	for(unsigned int i = 0; i < NUM_CASES; i++)
	{
		for(unsigned int j = 0; j < POINTS_NEEDED * 4; j++)
		{
			in >> num;

			if(j%4 > 1)
				inputs[i].push_back(num);
			else
				outputs[i].push_back(num);
		}
	}
}

vector<vector<double>> DataHandler::getInput()
{
	return inputs;
}

vector<vector<double>> DataHandler::getOutput()
{
	return outputs;
}

DataSet::DataSet(int feelerLen, float distance, CL_Angle angle)
{
	setDist(feelerLen, distance);
	setAngle(angle);
	setThrust();
	setTurn();
}

void DataSet::setDist(int feelerLen, float distance)
{
	//Set a "ceiling" of 1
	if(distance > feelerLen)
		dist = 1;
	else
		dist = distance/feelerLen;
}

void DataSet::setAngle(CL_Angle in)
{
	//Convert to degrees for ease of use
	double ang = in.to_degrees();

	angle = ((ang + 180) - 360) / 360;
}

void DataSet::setTurn()
{
	//Right is 1
	if(Resources::ic.get_keyboard().get_keycode(CL_KEY_D) ||
		Resources::ic.get_keyboard().get_keycode(CL_KEY_RIGHT))
		turn = 1;
	//Left is -1
	else if(Resources::ic.get_keyboard().get_keycode(CL_KEY_A) ||
		Resources::ic.get_keyboard().get_keycode(CL_KEY_LEFT))
		turn = -1;
	else
		turn = 0;
}

void DataSet::setThrust()
{
	//Up is 1
	if(Resources::ic.get_keyboard().get_keycode(CL_KEY_W) ||
		Resources::ic.get_keyboard().get_keycode(CL_KEY_UP))
		thrust = 1;
	//Down is -1
	else if(Resources::ic.get_keyboard().get_keycode(CL_KEY_S) ||
		Resources::ic.get_keyboard().get_keycode(CL_KEY_DOWN))
		thrust = -1;
	else
		thrust = 0;
}

string DataSet::toString()
{
	ostringstream stream;
	stream << thrust << " " << turn << " " << dist << " " << angle << " ";
	return stream.str();
}

bool DataSet::operator==(const DataSet &ds)
{
	if(thrust == ds.thrust)
		if(turn == ds.turn)
			return true;

	return false;
}

DataSet &DataSet::operator+(const DataSet &ds)
{
	dist = (dist + ds.dist) / 2;
	angle = (angle + ds.angle) / 2;

	return *this;
}