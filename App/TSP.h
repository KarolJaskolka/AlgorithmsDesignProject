#pragma once

#include "Matrix.h"

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
	bool isVisited(std::vector<int> visited, int city);
	void remove(std::vector<Matrix*> &list, int id);
	Matrix* findBetter(std::vector<Matrix*> &list, Matrix *best, Matrix *reduced, int &nextStart);

public:

	// constructors

	TSP();
	TSP(std::string fileName);

	// destructor

	~TSP();

	// algorithms

	int nearestNeighbour(int start);
	int kNearestNeighbour();
	int BranchAndBound();
	int BranchAndBoundPenalty(); // does not work properly
	int bruteForceSTL();
	int bruteForceTree();
	int bruteForceTreeFaster();
	int bruteForceSwap();
	int HeldKarp(); // not implemented yet
	
	// paths' display methods

	void showBestPath();
	void showTempPath();
	void showPointPath();

};

