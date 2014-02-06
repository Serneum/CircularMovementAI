/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 3/19/2011
* File: NNetwork.h
* Purpose: Defines all nodes, layers and network information
*****************************************************************/

#ifndef NET_INCLUDE
#define NET_INCLUDE

#define BIAS -1
#define ERROR_RANGE .003

#include <vector>
using namespace std;

//Neurons
class NNode
{
public:
	vector<double> inWeights;
	int numIn;
	double error;
	double activation;

	NNode(int inputs);
	NNode(){};
};

//Neuron Layers
class NLayer
{
public:
	int numNodes;
	vector<NNode> nodeList;

	NLayer(int nodes, int numIn);
	NLayer(){};
};

//Neural Network
class NNetwork
{
public:
	NNetwork(){};
	void init();
	vector<double> update(vector<double> &in);
	NNetwork(int inputs, int outputs, int hNodes, double lRate);
	bool trainingCycle(vector<vector<double>> &In, vector<vector<double>> &Out);
	vector<NLayer> layerList;
	int numIn;
	bool train();
	void printNN(string s);
	void readNN(string s);
private:
	inline double sigmoid(double input);
	int numOut;
	int numHNodes;
	int numHLayers;
	int numLoop;
	double error;
	double learnRate;
	bool trained;
};

#endif