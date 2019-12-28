#pragma once

#include "Matrix.h"
#include "TabuList.h"

class TSP
{
private:

	Matrix *problem;
	Matrix *reducedProblem;

	std::vector<int> bestPath;
	std::vector<int> tempPath;

	// private methods

	void myPermutationTree(int start, std::vector<int> order, int &min);
	void myPermutationTreeFaster(int start, std::vector<int> order, std::vector<int> next, int &min);
	void myPermutationSwap(std::vector<int> order, int left, int right, int &min);
	int *generateOrder(int n);
	bool equals(std::vector<int> order, std::vector<int> initOrder);
	int localMinimum(std::vector<int> &order, bool swapN, TabuList *tabuList, bool aspiration);
	int localMinimum(std::vector<int> &order);
	bool isVisited(std::vector<int> visited, int city);
	void remove(std::vector<Matrix*> &list, int id);
	int getSolutionLength(std::vector<int>order);
	void neighbourhoodSwap(std::vector<int> &order, int x, int y);
	void neighbourhoodInsert(std::vector<int> &order, int x, int y);
	Matrix* findBetter(std::vector<Matrix*> &list, Matrix *best, Matrix *reduced, int &nextStart);
	std::vector<int> generateOrderVector();
	
	// Genetic Algorithm 
	std::vector<std::vector<int>> initPopulation(int populationSize);
	void PartiallyMappedCrossover(std::vector<int> &p, std::vector<int> &q);
	void OrderedCrossover(std::vector<int> &p, std::vector<int> &q);
	void EdgeCrossover(std::vector<int> &p, std::vector<int> &q);
	void inversionMutation(std::vector<int> &individual);
	void insertionMutation(std::vector<int> &individual);
	void transpositionMutation(std::vector<int> &individual);
	int findCity(std::vector<int> child, std::vector<int> order);
	bool addPossible(std::vector<int> &child, int city, int index);
	int getNthPair(std::vector<std::pair<int, int>> pairs, int city, int n);

public:

	// constructors

	TSP();
	TSP(std::string fileName);

	// destructor

	~TSP();

	// algorithms

	int nearestNeighbour(int start);
	int kNearestNeighbour();
	int BranchAndBound(bool kNN);
	int BranchAndBoundPenalty(); // does not work properly
	int bruteForceSTL();
	int bruteForceTree();
	int bruteForceTreeFaster();
	int bruteForceSwap();
	int LocalSearch(int k);
	int TabuSearch(int iterations, int tabuSize, int cadence, bool SwapN, bool diversification, bool random, bool aspiration);
	int TabuSearchHybrid(int iterations, int tabuSize, int cadence, bool SwapN, bool diversification, bool random, bool aspiration);
	int GeneticAlgorithm(int populationSize);

	// paths' display methods
	void showBestPath();
	void showTempPath();
	void showPointPath();

};

