#include "pch.h"
#include "TSP.h"
#include "Matrix.h"
#include "TabuList.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

// constructor
TSP::TSP()
{
	problem = NULL;
	reducedProblem = NULL;
}

// constructor (load problem from file)
TSP::TSP(std::string fileName)
{
	problem = new Matrix(fileName);
	reducedProblem = NULL;
}

// destructor
TSP::~TSP()
{
	delete problem;
	delete reducedProblem;
}

// Nearest Neighbour Algorithm Implementation
int TSP::nearestNeighbour(int start)
{
	// full path length
	int length = 0;
	// path cost
	int cost;
	// current minimium founded cost
	int min;
	// list of visited cities
	vector<int> visited;
	// next city to visit
	int next;
	// new start city
	int newStart = start;
	// add to temporary best path (one of the k x NN)
	tempPath.push_back(newStart);
	// size - 1 paths to find (back to start will be added later)
	for (int i = 0; i < problem->size - 1; i++)
	{
		min = 2147483647; // max integer value

		// find neighbour with the lowest cost
		for (int j = 0; j < problem->size; j++) {

			// check only not visited cities (and not current start city)
			if (newStart != j && !isVisited(visited, j)) {
				// cost = path from newStart to j
				cost = problem->matrix[newStart][j];
				// check if lower than minimum
				if (cost < min) {
					min = cost;
					next = j; // j is next to visit
				}
			}
		}
		// add min path length
		length += min;
		// add to visited cities
		visited.push_back(newStart);
		// set next city as a start
		newStart = next;
		// add to temporary best path (one of the k x NN)
		tempPath.push_back(next);
	}

	// back to start
	length += problem->matrix[newStart][start];
	// return result
	return length;
}

// K Nearest Neighbour Algorithm Implementation
int TSP::kNearestNeighbour()
{
	int length = 0;
	int min = 2147483647;
	for (int i = 0; i < problem->size; i++) {
		tempPath.clear();
		length = nearestNeighbour(i);
		if (length < min) {
			min = length;
			bestPath.clear();
			bestPath = tempPath;
		}
	}
	return min;
}

// Branch And Bound Algorithm Implementation
int TSP::BranchAndBound(bool kNN)
{
	int id = 1; // used to remove best matrix from memory
	int start = 0; 
	int nextStart = 0;
	int upperBound;
	int lowerBound;

	vector<Matrix*> oldResults; // memory

	delete reducedProblem; 

	reducedProblem = new Matrix(problem); // init

	// choose upper bound algorithm
	if (kNN) {
		upperBound = kNearestNeighbour();
	}
	else {
		upperBound = nearestNeighbour(start);
	}
	
	lowerBound = reducedProblem->reduceMatrix(); // reduce matrix

	reducedProblem->path.push_back(start); // add start city to path

	int min;

	Matrix *best = new Matrix(); // matrix for best current solution

	for (int i = 0; reducedProblem->path.size() < problem->size; i++) {

		min = 2147483647;
		start = nextStart;

		for (int i = 0; i < problem->size; i++) {
			// find next city to visit
			if (!isVisited(reducedProblem->path, i)) {
				// estimate lowerBound of every matrix
				Matrix *matrix = new Matrix(reducedProblem);
				matrix->id = id++;
				matrix->reduceMatrix(start, i, lowerBound);
				// if better than upperBound save in memory
				if (matrix->bound < upperBound) {
					oldResults.push_back(matrix);
				}
				// check if current min lowerbound
				if (matrix->bound < min) {
					min = matrix->bound;
					best = matrix;
					nextStart = i;
				}
			}
		}

		// remove best from memory
		remove(oldResults, best->id);

		lowerBound = best->bound;

		// search (check memory) for better solution than current best 
		reducedProblem = findBetter(oldResults, best, reducedProblem, nextStart);

		lowerBound = reducedProblem->bound;

	}

	// solution path
	bestPath = reducedProblem->path;

	// clear vector
	oldResults.clear();
	oldResults.shrink_to_fit();

	return lowerBound;
}

// Branch And Bound (penalty) Algorithm Implementation
int TSP::BranchAndBoundPenalty()
{
	int length = 0;

	delete reducedProblem;

	reducedProblem = new Matrix(problem);

	for (int i = 0; i < problem->size; i++) {
		reducedProblem->reduceMatrix();
		reducedProblem->countAndFindPenalty();
	}

	for (int i = 0; i < reducedProblem->pathPoint.size(); i++) {
		length += problem->matrix[reducedProblem->pathPoint[i]->getX()][reducedProblem->pathPoint[i]->getY()];
	}

	return length;
}

// Brute Force Implementation (use STL next_permutation)
int TSP::bruteForceSTL()
{
	// order of cities to visit
	int *order = generateOrder(problem->size);
	// current path length
	int length;
	// minimum founded path length
	int min = 2147483647; // integer max value

	// check every possible solution
	// order changes in every loop (next_permutation)
	// (size-1)! times
	do {

#pragma region PathLength

		length = 0;

		for (int j = 0; j < problem->size - 1; j++) {
			length += problem->matrix[order[j]][order[j + 1]];
		}

		length += problem->matrix[order[problem->size - 1]][order[0]];

		if (length < min) {
			min = length;
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}

		}

#pragma endregion

	} while (std::next_permutation(order + 1, order + problem->size));

	delete order;

	return min;
}

// Brute Force Implementation (Tree)
int TSP::bruteForceTree()
{
	vector<int> order;
	int min = 2147483647; // integer max value
	myPermutationTree(0, order, min);
	return min;
}

// Brute Force Implementation (Tree Faster)
int TSP::bruteForceTreeFaster() {
	vector<int> order;
	vector<int> next;
	for (int i = 1; i < problem->size; i++) {
		next.push_back(i);
	}
	int min = 2147483647; // integer max value
	myPermutationTreeFaster(0, order, next,  min);
	return min;
}

// Brute Force Implementation (Swap)
int TSP::bruteForceSwap()
{
	vector<int> order;
	for (int i = 0; i < problem->size; i++) {
		order.push_back(i);
	}
	int min = 2147483647; // integer max value
	myPermutationSwap(order, 1, problem->size - 1, min);
	return min;
}

// Local Search Algorithm Implementation
int TSP::LocalSearch(int k) {

	// use k Nearest Neighbour Algorithm to set initial solution
	// after finding solution sets field bestPath
	kNearestNeighbour();

	// initial solution x^0
	vector<int> order = bestPath;

	// solution x'
	vector<int> orderPrime;
	
	int min = getSolutionLength(order);

	// for k = 1,2,3... k-1
	for (int i = 0; i < k; i++) {

		// generate new solution
		for (int i = 1; i < problem->size; i++) {
			for (int j = 1; j < problem->size; j++) {
				
				if (i != j) {
					int x = i;
					int y = j;

					orderPrime = order;

					neighbourhoodSwap(orderPrime, x, y);

					int length = getSolutionLength(orderPrime);

					// if solution x' better than current one

					if (length < min) {
						min = length;
						order = orderPrime;
					}
				}
			}
		}
	}

	bestPath = order;

	return min;
}

// Tabu Search Algorithm Implementation
int TSP::TabuSearch(int iterations, int tabuSize, int cadence, bool SwapN, bool diversification, bool random) {

	TabuList *tabuList = new TabuList(tabuSize, cadence);

	// initial solution x^0
	vector<int> order;

	if (!random) {
		// use k Nearest Neighbour Algorithm to set initial solution
		// after finding solution sets field bestPath
		kNearestNeighbour();
		order = bestPath;
	}
	else {
		order = generateOrderVector();
	}

	// solution x'
	vector<int> orderPrime;

	int min = getSolutionLength(order);

	int changes = 0;
	int streak = 0;

	bestPath = order;

	// for k = 1,2,3... k-1
	for (int k = 0; k < iterations; k++) {

		// generate new solution
		for (int i = 1; i < problem->size; i++) {
			for (int j = 1; j < problem->size; j++) {

				if (i != j) {
					
					int x = i;
					int y = j;

					orderPrime = order;

					// swap or insert neighbourhood
					if (SwapN) {
						neighbourhoodSwap(orderPrime, x, y);
					}
					else {
						neighbourhoodInsert(orderPrime, x, y);
					}

					int length = getSolutionLength(orderPrime);

					// if solution x' better than current one

					if (length < min) {
						int tabuPos = tabuList->find(x, y);
						// apiration criterium
						// remove from tabu list if solution is better
						if (tabuPos != -1) {
							tabuList->removeAtIndex(tabuPos);
						}
						// add to tabu list
						// if max size inner method removeFirst()
						else {
							tabuList->add(x, y);
						}
						// new best path length
						min = length;
						// new solution
						order = orderPrime;
						bestPath = orderPrime;
						changes++;
					}
				}
			}
		}
		// if no changes generate random solution as a new order
		if (diversification) {
			if (changes == 0) {
				streak++;
				if (streak > cadence) {
					order = generateOrderVector();
					streak = 0;
				}
			}
			else {
				changes = 0;
				streak = 0;
			}
		}
		// cadence-- of each element 
		// if cadence == 0 inner remove
		tabuList->decreaseAll();
	}

	delete tabuList;

	return min;
}

// Held Karp Algorithm Implementation
int TSP::HeldKarp()
{
	// to be done ...
	return 0;
}

// show best path
void TSP::showBestPath() {
	for (int i = 0; i < bestPath.size(); i++) {
		std::cout << bestPath[i] << " -> ";
	}
	std::cout << bestPath[0] << endl;
}

// show best path (NN algorithm)
void TSP::showTempPath() {
	for (int i = 0; i < tempPath.size(); i++) {
		std::cout << tempPath[i] << " -> ";
	}
	std::cout << tempPath[0] << endl;
}

// show best path (BnB [penalty] algorithm)
void TSP::showPointPath() {
	for (int i = 0; i < reducedProblem->pathPoint.size(); i++) {
		std::cout << reducedProblem->pathPoint[i]->getX() << " -> "
			<< reducedProblem->pathPoint[i]->getY() << " | ";
	}
	std::cout << endl;
}

// own permutation (size-1)! times, first digit is start
void TSP::myPermutationTree(int start, vector<int> order, int &min) {

	order.push_back(start);

	if (order.size() == problem->size) {

#pragma region PathLength

		int length = getSolutionLength(order);

		if (length < min) {
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}
			min = length;
		}

#pragma endregion

		return;
	}

	for (int i = 0; i < problem->size; i++) {
		if (!isVisited(order, i)) {
			vector<int> list = order;
			myPermutationTree(i, list, min);
		}
	}

}

// own permutation (size-1)! times, first digit is start (Faster)
void TSP::myPermutationTreeFaster(int start, std::vector<int> order, std::vector<int> next, int &min) {

	order.push_back(start);

	next.erase(std::remove(next.begin(), next.end(), start), next.end());

	if (next.size() == 0) {

#pragma region PathLength

		int length = getSolutionLength(order);

		if (length < min) {
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}
			min = length;
		}

#pragma endregion

		return;
	}

	for (int i = 0; i < next.size(); i++) {
		myPermutationTreeFaster(next[i], order, next, min);
	}
}

// swap permutation
void TSP::myPermutationSwap(vector<int> order, int left, int right, int &min) {

	if (left == right) {

#pragma region  PathLength

		int length = getSolutionLength(order);

		if (length < min) {
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}
			min = length;
		}

#pragma endregion

		return;
	}

	else {
		for (int i = left; i <= right; i++) {

			swap(order[left], order[i]);

			myPermutationSwap(order, left + 1, right, min);

			swap(order[left], order[i]);

		}
	}

}

// return n element array [ 0,1, ... , n-1]
int * TSP::generateOrder(int n)
{
	int *order = new int[n];
	for (int i = 0; i < n; i++) {
		order[i] = i;
	}
	return order;
}

// check if city has been visited
bool TSP::isVisited(std::vector<int> visited, int city)
{
	for (int elem : visited) {
		if (elem == city) {
			return true;
		}
	}
	return false;
}

// remove matrix from list with given id
void TSP::remove(std::vector<Matrix*> &list, int id) {
	for (int i = 0; i < list.size(); i++) {
		if (list[i]->id == id) {
			list.erase(list.begin() + i);
		}
	}
}

// check if better solution is available
Matrix* TSP::findBetter(std::vector<Matrix*> &list, Matrix *best, Matrix *reduced, int &nextStart) {
	
	reduced = best;
	
	int min = best->bound;

	for (Matrix* elem : list) {

		if (elem->bound < min) {

			min = elem->bound;
			int id = elem->id;
			list.push_back(reduced);
			reduced = elem;
			remove(list, id);
			nextStart = reduced->getLastCity();

		}

	}
	return reduced;
}

// return path cost of given solution
int TSP::getSolutionLength(std::vector<int>order) {
	
	int length = 0;

	for (int i = 0; i < problem->size - 1; i++) {
		length += problem->matrix[order[i]][order[i + 1]];
	}

	length += problem->matrix[order[problem->size - 1]][order[0]];

	return length;

}

// swap two elements
void TSP::neighbourhoodSwap(std::vector<int> &order, int x, int y) {
	int temp = order[x];
	order[x] = order[y];
	order[y] = temp;
}

// insert second element before first 
void TSP::neighbourhoodInsert(std::vector<int> &order, int x, int y) {

	// y must be bigger number
	if (x > y) {
		int z = x;
		x = y;
		y = z;
	}

	int temp = order[y];

	for (int i = y; i > x; i--) {
		neighbourhoodSwap(order, i, i-1);
	}

	order[x] = temp;

}

std::vector<int> TSP::generateOrderVector() {
	vector<int> cities;
	vector<int> order;
	for (int i = 0; i < problem->size; i++) {
		cities.push_back(i);
	}

	for (int i = 0; i < problem->size; i++) {
		int city = rand() % cities.size();
		order.push_back(cities[city]);
		cities.erase(cities.begin() + city);
	}
	return order;
}