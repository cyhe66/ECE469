#include <cmath>
#include <string>
#include <vector>

class NeuralNetwork{
	public:
		NeuralNetwork();//constructor
	//	~NeuralNetwork();//destructor

		typedef struct{
			double value;
		}inputNode;

		typedef struct{	
			bool isHidden;
			bool isOutput;

			double in;			//input sum (double)
			double activation;	//obtained from sigmoid of 'in' (activation
			double delta;		// obtained by backprop
		//information of a node stored as a pair<pair<int,int>, double>>
		//where pair<int fromNode, int toNode>,  and double is the weight 
			std::vector<std::pair<int,double>> inLinks;
			std::vector<std::pair<int,double>> outLinks;
		}Node;

		void configureNN();
		void loadFromFile(std::string const& filepath);
		void trainNeuralNet(std::string const& filepath, double alpha, unsigned int epochs);
	
		//void saveToFile(std::string const&);
		void print();

	private:
		double** inputs;//pointer to a double that will be 2-d array
		double** outputs;
		double** i_h;//weights of links from input layer to the hidden layer
		double** h_o;//weights of links from hidden layer to the output layer
		
		int numInputNodes;//
		int numHiddenNodes;
		int numOutputNodes;
		std::vector<NeuralNetwork::inputNode> v_inputNodes; // vector of inputN
		std::vector<NeuralNetwork::Node> v_hiddenNodes;//vec of hiddenN
		std::vector<NeuralNetwork::Node> v_outputNodes;//vec of outputN


		double sigmoid(double x);
		double sigmoidPrime(double x);

};



