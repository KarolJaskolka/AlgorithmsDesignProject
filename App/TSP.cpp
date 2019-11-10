#include "pch.h"
#include "TSP.h"
#include "Matrix.h"
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
}

// constructor (load problem from file)
TSP::TSP(std::string fileName)
{
	problem = new Matrix(fileName);
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
int TSP::BranchAndBound()
{
	int id = 1;
	int length = 0;
	int start = 0;
	int nextStart = 0;

	vector<Matrix*> oldResults;

	delete reducedProblem;

	reducedProblem = new Matrix(problem);

	//int upperBound = nearestNeighbour(start);
	int upperBound = kNearestNeighbour();
	int lowerBound = reducedProblem->reduceMatrix();

	reducedProblem->path.push_back(start);

	int min;

	Matrix *best = new Matrix();

	for (int i = 0; reducedProblem->path.size() < problem->size; i++) {

		min = 2147483647;
		start = nextStart;

		for (int i = 0; i < problem->size; i++) {
			if (!isVisited(reducedProblem->path, i)) {
				Matrix *matrix = new Matrix(reducedProblem);
				matrix->id = id++;
				//cout << "\n BEFORE REDUCE MATRIX " << endl;
				//cout << "\n LOWER BOUND " << lowerBound << endl;
				//cout << "\n START " << start << endl;
				matrix->reduceMatrix(start, i, lowerBound);
				//cout << "To City " << i << " : " << matrix->bound << endl;
				//matrix->show();
				if (matrix->bound < upperBound) {
					oldResults.push_back(matrix);
				}
				if (matrix->bound < min) {
					min = matrix->bound;
					best = matrix;
					nextStart = i;
				}
			}
		}

		remove(oldResults, best->id);

		//cout << "\nMEMORY\n" << endl;

		//for (int i = 0; i < oldResults.size(); i++) {
		//	oldResults[i]->show();
		//	cout << "Path : ";
		//	oldResults[i]->showPath();
		//	cout << "ID : " << oldResults[i]->id << endl;
		//	cout << "Bound : " << oldResults[i]->bound << endl;
		//}

		//cout << "\nBEST\n" << endl;
		////best->show();
		//cout << "Path : ";
		//best->showPath();
		//cout << "ID : " << best->id << endl;
		//cout << "Bound : " << best->bound << endl;

		lowerBound = best->bound;

		reducedProblem = findBetter(oldResults, best, reducedProblem, nextStart);

		lowerBound = reducedProblem->bound;

		//reducedProblem = best;

		//cout << "\n LOWER BOUND " << lowerBound << endl;
		//cout << "--------------------------------------------------------------------------------------------------\n";
		//reducedProblem->showPath();

	}

	bestPath = reducedProblem->path;

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
	// size! times
	do {

		// ------------ PATH LENGTH ------------

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

		// ------------ PATH LENGTH ------------

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

		// ------------ PATH LENGTH ------------

		int length = 0;

		for (int j = 0; j < problem->size - 1; j++) {
			length += problem->matrix[order[j]][order[j + 1]];
		}

		length += problem->matrix[order[problem->size - 1]][order[0]];

		if (length < min) {
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}
			min = length;
		}

		// ------------ PATH LENGTH ------------

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

		// ------------ PATH LENGTH ------------

		int length = 0;

		for (int j = 0; j < problem->size - 1; j++) {
			length += problem->matrix[order[j]][order[j + 1]];
		}

		length += problem->matrix[order[problem->size - 1]][order[0]];

		if (length < min) {
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}
			min = length;
		}

		// ------------ PATH LENGTH ------------

		return;
	}

	for (int i = 0; i < next.size(); i++) {
		myPermutationTreeFaster(next[i], order, next, min);
	}
}

// swap permutation
void TSP::myPermutationSwap(vector<int> order, int left, int right, int &min) {

	if (left == right) {

		// ------------ PATH LENGTH ------------

		int length = 0;

		for (int j = 0; j < problem->size - 1; j++) {
			length += problem->matrix[order[j]][order[j + 1]];
		}

		length += problem->matrix[order[problem->size - 1]][order[0]];

		if (length < min) {
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}
			min = length;
		}

		// ------------ PATH LENGTH ------------

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
	//cout << list.size() << endl;
	for (Matrix* elem : list) {
		//cout << "Elem: " << elem->bound << endl;
		//cout << "Min : " <<  min << endl;
		//elem->showPath();
		if (elem->bound < min) {
			min = elem->bound;
			int id = elem->id;
			list.push_back(reduced);
			reduced = elem;
			remove(list, id);
			//cout << "-------------------------------------------------------------------------------------------------" << endl;
			//cout << "FOUND" << endl;
			//reduced->show();
			//reduced->showPath();
			//cout << "Bound Memory: " << reduced->bound << endl;
			//cout << "New Start City : " << reduced->getLastCity();
			nextStart = reduced->getLastCity();
			//cout << "-------------------------------------------------------------------------------------------------" << endl;

		}

	}
	return reduced;
}
