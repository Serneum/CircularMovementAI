/*****************************************************************
* Author: Chris Rees, Wilfredo Velazquez
* Date: 3/19/2011
* File: NNetwork.cpp
* Purpose: Creates a neural network for neuroevolution or 
* backpropagation
*****************************************************************/

#include "../headers/NNetwork.h"
#include "../headers/DataHandler.h"
#include "../headers/GameResources.h"
#include <math.h>

//This is the sigmoid function that is used in neural networks/backpropagation
inline double NNetwork::sigmoid(double input)
{
	return (1/(1 + exp(-input)));
}

//Initialize the node. The initial activation/error is always 0
NNode::NNode(int inputs) : numIn(inputs + 1), activation(0), error(0)
{
	for(int i = 0; i < inputs + 1; i++)
		inWeights.push_back(Resources::RandomClamped());
}

//Initialize the layer and add the nodes.
NLayer::NLayer(int nodes, int numIn) : numNodes(nodes)
{
	for(int i = 0; i < nodes; i++)
		nodeList.push_back(NNode(numIn));
}

//Initialize the network and add the layers
NNetwork::NNetwork(int inputs, int outputs, int hNodes, double lRate) : numIn(inputs), numOut(outputs),
																		numHNodes(hNodes), learnRate(lRate),
																		numHLayers(1), error(9999), trained(false),
																		numLoop(0)
{
	//Create the hidden layers in the network
	if(numHLayers > 0)
	{
		//Create the initial layer
		layerList.push_back(NLayer(numHNodes, numIn));

		//Create the hidden layers, each with the number of inputs 
		//equal to the number of nodes around it
		for(int i = 0; i < numHLayers - 1; i++)
			layerList.push_back(NLayer(numHNodes, numHNodes));

		//Create the output layer
		layerList.push_back(NLayer(numOut, numHNodes));
	}
	//Just create the output layer
	else
		layerList.push_back(NLayer(numOut, numIn));
}

//Used in training cases
void NNetwork::init()
{
	numLoop = 0;
	error = 9999;
	//Initialize each weight in each node in each layer to a value -1 < val < 1
	for(int i = 0; i <= numHLayers; i++)
		for(int j = 0; j < layerList[i].numNodes; j++)
			for(int k = 0; k < layerList[i].nodeList[j].numIn; k++)
				layerList[i].nodeList[j].inWeights[k] = Resources::RandomClamped();
}

vector<double> NNetwork::update(vector<double> &in)
{
	vector<double> out;
	double input = 0, counter = 0, score = 0;

	//Error. Invalid number of inputs
	if(in.size() != numIn)
		return out;

	for(int i = 0; i < numHLayers + 1; i++)
	{
		//Set the inputs equal to the outputs
		if(i > 0)
			in = out;

		//Clear the output vector for the next set of data
		out.clear();

		for(int j = 0; j < layerList[i].numNodes; j++)
		{
			//Store the current node in an easy-to-reference form
			NNode tmpNode = layerList[i].nodeList[j];
			int numInputs = tmpNode.numIn;

			for(int k = 0; k < numInputs - 1; k++)
				input += tmpNode.inWeights[k] * in[counter++];
			//Multiply by the BIAS (set to -1 to handle itself if it ever needed to be changed)
			input += tmpNode.inWeights[numInputs - 1] * BIAS;
			
			//Get the "score" from the sigmoid function
			score = tmpNode.activation = sigmoid(input);
			counter = 0;
			//Push the "score" onto the output vector
			out.push_back(score);
		}
	}
	//Return the newest set of output
	return out;
}

bool NNetwork::trainingCycle(vector<vector<double>> &In, vector<vector<double>> &Out)
{
	vector<double> outputs;
	error = 0;
	double errVal = 0;

	//Create an iterator to allow working through a vector of nodes
	vector<double>::iterator currWeight;
	vector<NNode>::iterator currOut, currHid;

	for(int i = 0; i < In.size(); i++)
	{
		for(int m = 0; m < In[i].size(); m+=2)
		{
			vector<double> twoInputs;
			twoInputs.push_back(In[i][m]);
			twoInputs.push_back(In[i][m+1]);

			outputs = update(twoInputs);
		}

		//An error ocurred.
		if(outputs.size() == 0)
			return false;

		for(int j = 0; j < numOut; j++)
		{
			errVal = (Out[i][j] - outputs[j]) * outputs[j] * (1 - outputs[j]);
			layerList[1].nodeList[j].error = errVal;

			//Update the total error for the network
			error += pow(Out[i][j] - outputs[j], 2);

			currWeight = layerList[1].nodeList[j].inWeights.begin();
			currHid = layerList[0].nodeList.begin();

			//Calculate weights for the network
			while(currWeight != layerList[1].nodeList[j].inWeights.end() - 1)
			{
				*currWeight += errVal * learnRate * currHid->activation;
				currWeight++;
				currHid++;
			}
			//Calculate the weight for the bias
			*currWeight += errVal * learnRate * BIAS;
		}
		currHid = layerList[0].nodeList.begin();
		int counter = 0;

		//Calculate the error for each individual node
		while(currHid != layerList[0].nodeList.end())
		{
			errVal = 0;

			currOut = layerList[1].nodeList.begin();

			//Calculate error based on input/weights from each of the connected nodes in the output layer
			while(currOut != layerList[1].nodeList.end())
			{
				errVal += currOut->error * currOut->inWeights[counter];
				currOut++;
			}
			
			errVal += currHid->activation * (1 - currHid->activation);
			//Calulate new weight based on error/learning rate
			for(int k = 0; k < numIn; k++)
				currHid->inWeights[k] += errVal * learnRate * In[i][k];
			
			currHid->inWeights[numIn] += errVal * learnRate * BIAS;
			
			counter++;
			currHid++;
		}
	}
	return true;
}

bool NNetwork::train()
{
	vector<vector<double>> In  = DataHandler::getInput();
	vector<vector<double>> Out = DataHandler::getOutput();

	//Initialize the network for training
	init();

	//Loop until the network has "learned" enough
	while(error > ERROR_RANGE)
	{
		//An error occurred
		if(!trainingCycle(In, Out))
			return false;

		numLoop++;
		Resources::cprintf("Loop:%d \tError:%lf", numLoop, error);
	}
	return trained = true;
}

void NNetwork::printNN(string s)
{
	//Create the filename
	string name = "docs/networks/";
	name.append(s);
	name.append(".net");

	//Open the file
	ofstream out;
	out.open(name.c_str(), ios::out);

	//Print out basic neural network info
	out << numIn << " " << numOut << " " << numHNodes << " " << numHLayers << " " << numLoop << endl;
	//Print out current error and learning rate
	out << error << " " << learnRate << endl;

	//For each layer...
	for(int i = 0; i < layerList.size(); i++)
	{
		//Print number of nodes
		out << layerList[i].numNodes << endl;
		//For each node...
		for(int j = 0; j < layerList[i].numNodes; j++)
		{
			//Print node information
			out << layerList[i].nodeList[j].numIn << " " << layerList[i].nodeList[j].error << " " << layerList[i].nodeList[j].activation << endl;
			//For each weight...
			for(int k = 0; k < layerList[i].nodeList[j].numIn; k++)
				//Print the weights between nodes
				out << layerList[i].nodeList[j].inWeights[k] << endl;
		}
	}
	out.close();
}

void NNetwork::readNN(string s)
{
	//Create the filename
	string name = "docs/networks/";
	name.append(s);
	name.append(".net");

	//Open the file
	ifstream in;
	in.open(name.c_str(), ios::in);
	
	//Read in the neural network info
	in >> numIn >> numOut >> numHNodes >> numHLayers >> numLoop;
	//Read in error and learning rate
	in >> error >> learnRate;
	
	//For each layer... (account for output layer)
	for(int i = 0; i <= numHLayers; i++)
	{
		layerList.push_back(NLayer());
		//Store how many nodes
		in >> layerList[i].numNodes;
		//For each node...
		for(int j = 0; j < layerList[i].numNodes; j++)
		{
			layerList[i].nodeList.push_back(NNode());
			//Read in the number of inputs, error and activation
			in >> layerList[i].nodeList[j].numIn >> layerList[i].nodeList[j].error >> layerList[i].nodeList[j].activation;
			//For each input...
			for(int k = 0; k < layerList[i].nodeList[j].numIn; k++)
			{
				layerList[i].nodeList[j].inWeights.push_back(0);
				//Store the weight corresponding to that input
				cin >> layerList[i].nodeList[j].inWeights[k];
			}
		}
	}
	in.close();
}