#pragma once

#include "Matrix.h"
#include "TabuList.h"

class TSP
{
private:

	Matrix *problem;
	Matrix *reducedProblem; // Branch And Bound

	std::vector<int> bestPath;
	std::vector<int> tempPath; // Nearest Neighbour

	// private methods

	int *generateOrder(int n);
	std::vector<int> generateOrderVector();
	int getSolutionLength(std::vector<int>order);

	// Brute Force
	void myPermutationTree(int start, std::vector<int> order, int &min);
	void myPermutationTreeFaster(int start, std::vector<int> order, std::vector<int> next, int &min);
	void myPermutationSwap(std::vector<int> order, int left, int right, int &min);

	// Branch And Bound
	Matrix* findBetter(std::vector<Matrix*> &list, Matrix *best, Matrix *reduced, int &nextStart);
	void remove(std::vector<Matrix*> &list, int id);
	bool isVisited(std::vector<int> visited, int city);

	// LocalSearch & TabuSearch
	int localMinimum(std::vector<int> &order, bool swapN, TabuList *tabuList, bool aspiration);
	int localMinimum(std::vector<int> &order);
	void neighbourhoodSwap(std::vector<int> &order, int x, int y);
	void neighbourhoodInsert(std::vector<int> &order, int x, int y);
	bool equals(std::vector<int> order, std::vector<int> initOrder);

	// Genetic Algorithm 
	void showPopulation(std::vector<std::vector<int>> population);
	std::vector<std::vector<int>> initPopulation(int populationSize);
	std::pair<std::vector<int>, std::vector<int>> PartiallyMappedCrossover(std::vector<int> p, std::vector<int> q);
	std::pair<std::vector<int>, std::vector<int>> OrderedCrossover(std::vector<int> p, std::vector<int> q);
	std::vector<int> EdgeCrossover(std::vector<int> p, std::vector<int> q);
	void inversionMutation(std::vector<int> &individual);
	void insertionMutation(std::vector<int> &individual);
	void transpositionMutation(std::vector<int> &individual);
	bool addPossible(std::vector<int> &child, int city, int index);
	int getNthPair(std::vector<std::pair<int, int>> pairs, int city, int n);
	int findReachedFromBoth(std::vector<std::vector<int>> &neighbourhood, int city);
	int findNearest(std::vector<std::vector<int>> &neighbourhood, int city);
	void removeFromNeighbourhood(std::vector<std::vector<int>> &neighbourhood, int city);
	void removeFromUnpicked(std::vector<int> &unpicked, int city);
	int randCityFromUnpicked(std::vector<int> &unpicked);
	int getFromOrder(std::vector<int> &order);

public:
	
	// constructors

	TSP();
	TSP(std::string fileName);

	// destructor

	~TSP();

	// algorithms

	// 1
	int BranchAndBound(bool kNN);
	int bruteForceSTL();
	int bruteForceTree();
	int bruteForceTreeFaster();
	int bruteForceSwap();

	int nearestNeighbour(int start);
	int kNearestNeighbour();

	// 2
	int LocalSearch(int k, double maxDuration);
	int TabuSearch(int iterations, int tabuSize, int cadence, bool SwapN, bool diversification, bool random, bool aspiration, double maxDuration);
	
	// 3
	int GeneticAlgorithm(int populationSize, int generations, int X, int mutation, int pM, int pC, double maxDuration);
	int AntColonyOptimization(int iterations, int set, double p, double alfa, double beta, double t0, double qt, double maxDuration);

	// paths' display methods
	void showBestPath();
	void showTempPath();

};