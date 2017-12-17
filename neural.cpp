//hello world
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <iostream> // std::cin, std::cout
#include <sstream>

#include "neural.hpp"

NeuralNetwork::NeuralNetwork(){
}

double NeuralNetwork::sigmoid(double x){
	return 1.0/(1.0+ exp(-x));
}

double NeuralNetwork::sigmoidPrime(double x){
	return sigmoid(x) * (1.0-sigmoid(x));
}

void NeuralNetwork::loadFromFile(std::string const& filepath){
	std::string tempString;
	std::ifstream inFile;
	std::vector<int> nodeInfo;
	inFile.open(filepath);

	if (!inFile.is_open()){
		std::cerr << "unable to open file :" << filepath<< ". " <<std::endl;
		exit(1);
	}
	
	std::getline(inFile, tempString);	
	std::istringstream iss(tempString);

	//extract the first 3 bits of information	
	std::string word;
	
	while (iss>>word){
		nodeInfo.push_back(std::stoi(word));
	}	

	//store the word vector to gobal later
	numInputNodes = nodeInfo[0];
	numHiddenNodes = nodeInfo[1];
	numOutputNodes = nodeInfo[2];

	i_h = new double*[nodeInfo[1]];//dynamic array (size #of input nodes) of ptrs to int
	for ( int ii = 0; ii < nodeInfo[1]; ii++){ //5
		i_h[ii] = new double[nodeInfo[0]+1];//30+1
	}

	h_o = new double*[nodeInfo[2]];
	for ( int ii = 0; ii < nodeInfo[2]; ii++){ //1
		h_o[ii] = new double[nodeInfo[1]+1];//6
	}
	
	for (int ii = 0; ii < nodeInfo[1] + nodeInfo[2];ii++){//read all the lines
		if (ii < nodeInfo[1]){ // this is the first x nodes, part of the input-> hidden layer
			std::getline(inFile, tempString);
			iss = std::istringstream(tempString);
			int counter = 0; 
			while (iss>> word){
				i_h[ii][counter] = std::stod(word);
				counter++;
			}			
			counter = 0;
		}
		//part of the weights of the edges from the hidden nodes to the output nodes
		else{
			std::getline(inFile, tempString);
			iss = std::istringstream(tempString);
			int counter = 0;
			while( iss>>word){
				h_o[(ii-nodeInfo[1])][counter] = std::stod(word);
				std::cout<<ii<<std::endl;
				counter++;
			}
			counter = 0;
		}
	}
}

void NeuralNetwork::configureNN(){
	for (int ii = 0; ii < numHiddenNodes; ii++){
		//create a new node
		v_hiddenNodes.push_back(NeuralNetwork::Node());
		v_hiddenNodes[ii].isHidden = true;
		v_hiddenNodes[ii].isOutput = false;
		
		for (int jj = 0; jj < numInputNodes+1; jj++){ // jj is the number of the input node
			std::pair <int,double> link; //initialize link
			link.first = jj;//the input node#
			link.second = i_h[ii][jj];	
			v_hiddenNodes[ii].inLinks.push_back(link);//add the link to the vector
		}
	}	
	//to debug print statements to see if the hidden nodes were correctly generated;
	/*
	std::cout<<"**************DEBUGGING for HIDDEN NODES*************"<<std::endl;
	std::cout<<"Number of Hidden Nodes: "<< numHiddenNodes<<std::endl;
	std::cout<<"Number of Input Nodes: "<< numInputNodes<<std::endl;
	std::cout<<"# of pushed back nodes in vector: " << v_hiddenNodes.size()<<std::endl;

	for (std::vector<NeuralNetwork::Node>::iterator it = v_hiddenNodes.begin(); it !=v_hiddenNodes.end(); ++it){
		//node->inlinks accesses the inlinks Vector
		for (std::vector<std::pair<int,double>>::iterator itt = it->inLinks.begin(); itt != it->inLinks.end(); ++itt){
			std::cout<< "From Input Node: "<< itt->first;
			std::cout<< " with Weight of: "<< itt->second;
			std::cout<<std::endl;
		}
	}
	*/
	//these do not have outlinks -- they just have a value
	for (int ii = 0; ii < numOutputNodes; ii++){
		v_outputNodes.push_back(NeuralNetwork::Node());
		v_outputNodes[ii].isHidden = false;
		v_outputNodes[ii].isOutput = true;
	
		//create all the vectors	
		for (int jj = 0; jj < numHiddenNodes+1; jj++){
			std::pair<int, double> link;	
			link.first = jj;
			link.second = h_o[ii][jj];
			v_outputNodes[ii].inLinks.push_back(link);
		}	
	}			
	//to debug print statements to see if the hidden nodes were correctly generated;
	/*
	std::cout<<"**************DEBUGGING for OUTPUT NODES*************"<<std::endl;
	std::cout<<"Number of output Nodes: "<< numOutputNodes<<std::endl;
	std::cout<<"Number of Hidden Nodes: "<< numHiddenNodes<<std::endl;
	std::cout<<"# of pushed back nodes in vector: " << v_outputNodes.size()<<std::endl;

	for (std::vector<NeuralNetwork::Node>::iterator it = v_outputNodes.begin(); it !=v_outputNodes.end(); ++it){
		//node->inlinks accesses the inlinks Vector
		for (std::vector<std::pair<int,double>>::iterator itt = it->inLinks.begin(); itt != it->inLinks.end(); ++itt){
			std::cout<< "From hidden Node: "<< itt->first;
			std::cout<< " with Weight of: "<< itt->second;
			std::cout<<std::endl;
		}
	}
	*/

}


void trainNeuralNet(std::string const& filepath, double alpha, unsigned int epochs){
	//load in the input//output vectors
	
}











//this is a bogus hardcoded test for wdbc_train.txt to see if I read in the data correctly
void NeuralNetwork::print(){
	int a = 5;
	int b = 30;
	int c = 1;

	for (int ii = 0; ii < 5; ii++){
		for (int jj = 0; jj < b+1; jj++){
			std::cout<<" "<< i_h[ii][jj] << " ";
		}	
		std::cout << std::endl;
	}

	std::cout << " ----------" << std::endl;

	for ( int kk = 0; kk < 6; kk++){
		std::cout<<" "<<h_o[0][kk] << " ";
	}
	std::cout <<std::endl;
	std::cout << " ---------------------" << std::endl;
}

int main(){
	NeuralNetwork nnet;
	std::string filename;

	std::cout<<"Please enter a filename: "<<std::endl;
	std::cin>> filename;
	
	nnet.loadFromFile(filename);
	nnet.configureNN();
	nnet.print();
	return 0;
}


