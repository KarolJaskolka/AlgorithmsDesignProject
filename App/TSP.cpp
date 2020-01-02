#include "pch.h"
#include "TSP.h"
#include "Matrix.h"
#include "TabuList.h"
#include "Ant.h"
#include "Timer.h"
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
	// clear tempPath
	tempPath.clear();
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
	
	int globalMin = getSolutionLength(order);

	// for k = 1,2,3... k-1
	for (int i = 0; i < k; i++) {

		int localMin = localMinimum(order);

		if (localMin < globalMin) {
			globalMin = localMin;
			bestPath = order;
		}

	}

	return globalMin;
}

// Tabu Search Algorithm Implementation (number of iterations, size of tabuList, cadence length of every city on tabuList,
// neighbourhood true-Swap, false-Insert, restart after no changes during few iterations, 
// random initial solution or using k Nearest Neighbour, aspiration criterium enabled/diabled)
int TSP::TabuSearch(int iterations, int tabuSize, int cadence, bool SwapN, bool diversification, bool random, bool aspiration) {

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

	// copy of init order
	vector<int> initOrder = order;

	int globalMin = getSolutionLength(order);

	int streak = 0; // better solution not found 

	bestPath = order;

	TabuList *tabuList = new TabuList(tabuSize, cadence);

	// for k = 1,2,3... k-1
	for (int k = 0; k < iterations; k++) {

		int localMin = localMinimum(order, SwapN, tabuList, aspiration);

		if (localMin < globalMin) {
			globalMin = localMin;
			bestPath = order;
			streak = 0;
		}
		else {
			streak++;
		}

		// cadence-- of each element 
		// if cadence == 0 inner remove
		tabuList->decreaseAll();

		// if diversification enabled check critical events
		// then generate random solution as a new order

		// back to init order
		if (equals(order, initOrder)) {
			if (diversification) {
				order = generateOrderVector();
				tabuList->clear();
			}
			else {
				break;
			}
		}
		// found again the same best solution
		if (streak > 0 && equals(order, bestPath)) {
			if (diversification) {
				order = generateOrderVector();
				tabuList->clear();
			}
			else {
				break;
			}
		}
		// no progress
		if (streak > 250 && streak > problem->size) {
			if (diversification) {
				order = generateOrderVector();
				tabuList->clear();
			}
		}

	}

	delete tabuList;

	return globalMin;
}

// Genetic Algorithm implementation
// X : [0 - OX] [1 - PMX] [2 - EX]
// mutation : [0-invert] [1-insert] [2-swap]
// pM : mutation probability
// pC : crossover probability
int TSP::GeneticAlgorithm(int populationSize, int generations, int X, int mutation, int pM, int pC) {

	// Init population
	vector<vector<int>> population = initPopulation(populationSize);
	
	// Fitness getSolutionLength
	// Sort by path length
	std::sort(population.begin(), population.end(), [this](const vector<int> & a, const vector<int> & b)
				{ return this->getSolutionLength(a) < this->getSolutionLength(b); });

	for (int i = 0; i < generations; i++) {

		// Selection
		if (population.size() > populationSize) {
			population.erase(population.begin() + populationSize, population.end());
		}
		
		int loop = populationSize;
		// After removing duplicates population.size() might be lower
		if (populationSize > population.size()) {
			loop = population.size();
		}

		// Crossover
		for (int j = 0; j < loop; j+=2) {
			// e.g. pC = 80 -> 0.8
			if ((rand() % 100) < pC) {
				
				pair<vector<int>, vector<int>> offspring;

				int x = rand() % loop;
				int y = rand() % loop;

				if (x == y) {
					x = 0;
					y = 1;
				}

				switch (X) {
				case 0:
					offspring = OrderedCrossover(population[x], population[y]);
					break;
				case 1:
					offspring = PartiallyMappedCrossover(population[x], population[y]);
					break;
				default:
					offspring.first = EdgeCrossover(population[x], population[y]);
					offspring.second = EdgeCrossover(population[x], population[y]);
					break;
				}

				population.push_back(offspring.first);
				population.push_back(offspring.second);
			}
		}

		// Mutation
		for (int j = 0; j < population.size(); j++) {
			// e.g. pM = 1 -> 0.01 %
			if (rand() % 100 < pM) {
				switch (mutation) {
				case 0:
					inversionMutation(population[j]); // invert
					break;
				case 1:
					insertionMutation(population[j]); // insert
					break;
				default:
					transpositionMutation(population[j]); // swap
					break;
				}
			}
		}

		// New population
		std::sort(population.begin(), population.end(), [this](const vector<int> & a, const vector<int> & b)
					{ return this->getSolutionLength(a) < this->getSolutionLength(b); });
		
		// Remove duplicates
		population.erase(unique(population.begin(), population.end()), population.end());

	}

	bestPath = population[0];

	int length = getSolutionLength(population[0]);

	population.clear();
	population.shrink_to_fit();

	return length;
}

// Ant Colony Optimization implementation 
// set : [0 - DAS] [1 - QAS] [2 - CAS] 
// p : evaporation parameter [0,1]
// alfa : pheromone regulation parameter 
// beta : criterium (1/distance) regulation parameter
// t0 : initial pheromone quantity
// qt : quantity of pheromone to put down on edge
int TSP::AntColonyOptimization(int iterations, int set, double p, double alfa, double beta, double t0, int qt) {

	// Init pheromone map
	double ** pheromoneMap;
	pheromoneMap = new double*[problem->size];

	for (int i = 0; i < problem->size; i++) {
		pheromoneMap[i] = new double[problem->size];
		for (int j = 0; j < problem->size; j++) {
			if (j != i) { pheromoneMap[i][j] = t0; }
			else { pheromoneMap[i][j] = 0.0; }
		}
	}

	vector<Ant*> ants;

	// ants.size() == problem->size

	// Init Ant Colony
	for (int i = 0; i < problem->size; i++) {
		ants.push_back(new Ant(problem->matrix, pheromoneMap, problem->size));
	}

	for (int k = 0; k < iterations; k++) {

		// Select random start city for each ant
		
		vector<int> order = generateOrderVector(); // cities in random order
		
		for (int i = 0; i < ants.size(); i++) {
			ants[i]->clearVisited();
			ants[i]->setStartCity(order[i]);
		}

		// Run !
		for (int i = 0; i < ants.size(); i++) {
			ants[i]->run(alfa, beta);
		}

		// Update pheromone

		// Evaporate
		for (int i = 0; i < problem->size; i++) {
			for (int j = 0; j < problem->size; j++) {
				if (i != j) {
					pheromoneMap[i][j] *= p;
				}
			}
		}

		// Put pheromone down
		for (int i = 0; i < ants.size(); i++) {
			
			vector<int> path = ants[i]->getPath();

			for (int j = 0; j < path.size() - 1; j++) {
				
				int first = path[j];
				int second = path[j + 1];

				switch (set) {
				case 0:
					// DAS - Ant Density
					pheromoneMap[first][second] += qt;
					break;
				case 1:
					// QAS - Ant Quantity
					{
					double distance = (double)problem->matrix[first][second];
					pheromoneMap[first][second] += qt / distance;
					}
					break;
				default:
					// CAS - Ant Cycle
					double cycle = (double)getSolutionLength(path);
					pheromoneMap[first][second] += qt / cycle;
					break;
				}
			}

		}
	}

	// best solution

	bestPath = ants[0]->getPath();

	int minLength = getSolutionLength(bestPath);

	for (int i = 0; i < ants.size(); i++) {

		int length = getSolutionLength(ants[i]->getPath());
		if (length < minLength) {
			minLength = length;
			bestPath = ants[i]->getPath();
		} 
	}

	// free memory

	for (int i = 0; i < ants.size(); i++) {
		delete ants[i];
	}

	ants.clear();
	ants.shrink_to_fit();

	for (int i = 0; i < problem->size; i++) {
		delete pheromoneMap[i];
	}
	delete pheromoneMap;

	return minLength;
}

// display population (Genetic Algorithm)
void TSP::showPopulation(std::vector<std::vector<int>> population) {
	//cout << "\nPopulation : \n";
	for (int i = 0; i < population.size(); i++) {
		for (int j = 0; j < problem->size; j++) {
			cout << population[i][j] << " ";
		}
		cout << " Length : " << getSolutionLength(population[i]) << endl;
	}
}

// display best path
void TSP::showBestPath() {
	for (int i = 0; i < bestPath.size(); i++) {
		std::cout << bestPath[i] << " -> ";
	}
	std::cout << bestPath[0] << endl;
}

// display best path (NN algorithm)
void TSP::showTempPath() {
	for (int i = 0; i < tempPath.size(); i++) {
		std::cout << tempPath[i] << " -> ";
	}
	std::cout << tempPath[0] << endl;
}

// own permutation (size-1)! times, first digit is start
void TSP::myPermutationTree(int start, vector<int> order, int &min) {

	order.push_back(start);

	if (order.size() == problem->size) {

		int length = getSolutionLength(order);

		if (length < min) {
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}
			min = length;
		}

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

		int length = getSolutionLength(order);

		if (length < min) {
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}
			min = length;
		}

		return;
	}

	for (int i = 0; i < next.size(); i++) {
		myPermutationTreeFaster(next[i], order, next, min);
	}
}

// swap permutation
void TSP::myPermutationSwap(vector<int> order, int left, int right, int &min) {

	if (left == right) {

		int length = getSolutionLength(order);

		if (length < min) {
			bestPath.clear();
			for (int i = 0; i < problem->size; i++) {
				bestPath.push_back(order[i]);
			}
			min = length;
		}

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

// return random generated order (std::vector)
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

// return local minimum found in neighbourhood (TabuSearch Algorithm)
int TSP::localMinimum(std::vector<int> &order, bool SwapN, TabuList *tabuList, bool aspiration) {

	int min = 2147483647;
	
	std::vector<int> bestLocalOrder;

	int elementOne;
	int elementTwo;

	bool aspirationUsed = false;

	// generating all solutions in neighbourhood
	for (int i = 1; i < problem->size; i++) {
		for (int j = 1; j < problem->size; j++) {

			if (i != j) {

				std::vector<int> copyOrder = order;

				if (SwapN) {
					neighbourhoodSwap(copyOrder, i, j);
				}
				else {
					neighbourhoodInsert(copyOrder, i, j);
				}

				int length = getSolutionLength(copyOrder);

				if (aspiration) {
					if (length < min) {
						
						if (tabuList->find(copyOrder[i], copyOrder[j])) {
							tabuList->findAndRemove(copyOrder[i], copyOrder[j]);
							aspirationUsed = true;
						} 
						
						min = length;
						bestLocalOrder = copyOrder;
						elementOne = copyOrder[i];
						elementTwo = copyOrder[j];
					}
				}
				else if(!tabuList->find(copyOrder[i], copyOrder[j])) {
					if (length < min) {
						min = length;
						bestLocalOrder = copyOrder;
						elementOne = copyOrder[i];
						elementTwo = copyOrder[j];
					}
				}
			}
		}
	}

	order = bestLocalOrder;
	
	if (!aspirationUsed) {
		tabuList->add(elementOne, elementTwo);
	}
	
	return min;
}

// return local minimum found in neighbourhood (LocalSearch Algorithm)
int TSP::localMinimum(std::vector<int> &order) {
	
	int min = 2147483647;

	std::vector<int> bestLocalOrder;

	// generating all solutions in neighbourhood
	for (int i = 1; i < problem->size; i++) {
		for (int j = 1; j < problem->size; j++) {

			if (i != j) {

				std::vector<int> copyOrder = order;

				neighbourhoodSwap(copyOrder, i, j);

				int length = getSolutionLength(copyOrder);

				if (length < min) {
					min = length;
					bestLocalOrder = copyOrder;
				}

			}
		}
	}

	order = bestLocalOrder;

	return min;
}

// compare two orders if equal return true
bool TSP::equals(std::vector<int> order, std::vector<int> initOrder) {
	for (int i = 0; i < order.size(); i++) {
		if (order[i] != initOrder[i]) {
			return false;
		}
	}
	return true;
}

// return population with random orders
std::vector<vector<int>> TSP::initPopulation(int populationSize) {
	
	std::vector<vector<int>> population;

	for (int i = 0; i < populationSize; i++) {
		population.push_back(generateOrderVector());
	}

	return population;
}

// PMX implementation (GeneticAlgorithm) 
std::pair<vector<int>, vector<int>> TSP::PartiallyMappedCrossover(std::vector<int> p, std::vector<int> q) {
	
	int k1 = rand() % problem->size;
	int k2 = rand() % problem->size;

	if (k1 == k2) {
		k1 = (problem->size / 2) - (problem->size / 4);
		k2 = (problem->size / 2) + (problem->size / 4);
	}

	if (k1 > k2) {
		int temp = k1;
		k1 = k2;
		k2 = temp;
	}

	std::vector<int> r;
	std::vector<int> s;

	// init offspring with -1
	for (int i = 0; i < problem->size; i++) {
		r.push_back(-1);
		s.push_back(-1);
	}

	std::vector<std::pair<int, int>> pairs;
	
	// transposition from parents to offspring between k1 and k2
	for (int i = k1; i < k2; i++) {
		r[i] = q[i];
		s[i] = p[i];
		// add to pairs
		pairs.push_back(std::make_pair(q[i], p[i]));
	}

	// put cities from parents without confilcts
	for (int i = 0; i < problem->size; i++) {
		if (r[i] == -1) {
			addPossible(r, p[i], i);
		}
		if (s[i] == -1) {
			addPossible(s, q[i], i);
		}
	}

	int city;

	// use pairs to put other cities
	for (int i = 0; i < problem->size; i++) {
		if (r[i] == -1) {
			
			int j = 1;
			do {
				city = getNthPair(pairs, p[i], j);
				j++;
			} while (addPossible(r, city, i) == false);

		}
		if (s[i] == -1) {

			int j = 1;
			do {
				city = getNthPair(pairs, q[i], j);
				j++;
			} while (addPossible(s, city, i) == false);

		}
	}

	return std::make_pair(r, s);

}

// OX implementation (GeneticAlgorithm) 
std::pair<vector<int>, vector<int>> TSP::OrderedCrossover(std::vector<int> p, std::vector<int> q) {
	
	int size = problem->size;

	int k1 = rand() % size;
	int k2 = rand() % size;
	
	if (k1 == k2) {
		k1 = (size / 2) - (size / 4);
		k2 = (size / 2) + (size / 4);
	}

	if (k1 > k2) {
		int temp = k1;
		k1 = k2;
		k2 = temp;
	}

	std::vector<int> r; // first offspring
	std::vector<int> s; // second offspring

	std::vector<int> o1; // p order from k2 [k2,...,n,0,...,k2-1]
	std::vector<int> o2; // q order from k2 [k2,...,n,0,...,k2-1]

	// set o1 and o2
	for (int i = 0; i < problem->size; i++) {
		o1.push_back(p[(k2 + i) % problem->size]);
		o2.push_back(q[(k2 + i) % problem->size]);
	}

	// init offspring with -1
	for (int i = 0; i < problem->size; i++) {
		r.push_back(-1);
		s.push_back(-1);
	}

	// transposition from parents to offspring between k1 and k2
	for (int i = k1; i < k2; i++) {
		r[i] = q[i];
		s[i] = p[i];
		removeFromUnpicked(o1, r[i]);
		removeFromUnpicked(o2, s[i]);
	}

	// copy from o1 and o2
	for (int i = 0; i < problem->size; i++) {
		if (r[(k2 + i) % problem->size] == -1) {
			r[(k2 + i) % problem->size] = getFromOrder(o1);
		}
		if (s[(k2 + i) % problem->size] == -1) {
			s[(k2 + i) % problem->size] = getFromOrder(o2);
		}
	}

	return std::make_pair(r, s);
}

// return front city from order (also delete city)
int TSP::getFromOrder(std::vector<int> &order) {
	int city = order[0];
	order.erase(order.begin());
	return city;
}

// add city when no confilcts
bool TSP::addPossible(std::vector<int> &child, int city, int index) {
	bool possible = true;

	for (int i = 0; i < problem->size; i++) {
		if (city == child[i]) possible = false;
	}

	if (possible) {
		child[index] = city;
	}

	return possible;
}

// find n-th pair (PMX)
int TSP::getNthPair(std::vector<std::pair<int, int>> pairs, int city, int n) {

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < pairs.size(); j++) {
			if (pairs[j].first == city) {
				city = pairs[j].second;
				pairs.erase(pairs.begin() + j);
				break;
			}
			if (pairs[j].second == city) {
				city = pairs[j].first;
				pairs.erase(pairs.begin() + j);
				break;
			}
		}
	}

	return city;
}

// EX implementation (GeneticAlgorithm)
std::vector<int> TSP::EdgeCrossover(std::vector<int> p, std::vector<int> q) {
	
	std::vector<int> offspring;

	std::vector<vector<int>> neighbourhood(p.size());

	for (int i = 0; i < p.size(); i++) {

		if (i == 0) {
			neighbourhood[p[i]].push_back(p[p.size() - 1]);
			neighbourhood[q[i]].push_back(q[q.size() - 1]);
		}
		else {
			neighbourhood[p[i]].push_back(p[(i - 1) % p.size()]);
			neighbourhood[q[i]].push_back(q[(i - 1) % q.size()]);
		}
		
		neighbourhood[p[i]].push_back(p[(i + 1) % p.size()]);
		neighbourhood[q[i]].push_back(q[(i + 1) % q.size()]);
		
	}

	vector<int> unpicked = generateOrderVector();

	int currentCity = unpicked[0];
	unpicked.erase(unpicked.begin());
	removeFromNeighbourhood(neighbourhood, currentCity);
	offspring.push_back(currentCity);

	int next;

	while (offspring.size() != p.size()) {

		if (neighbourhood[currentCity].size() > 0) {
			next = findReachedFromBoth(neighbourhood, currentCity);
			if (next == -1) {
				next = findNearest(neighbourhood, currentCity);
			}
		}
		else {
			next = randCityFromUnpicked(unpicked);
		}

		removeFromUnpicked(unpicked, next);
		removeFromNeighbourhood(neighbourhood, next);
		offspring.push_back(next);
		currentCity = next;
	}

	return offspring;
}

// find city reached from both parents (EX)
int TSP::findReachedFromBoth(std::vector<std::vector<int>> &neighbourhood, int city) {
	for (int i = 0; i < neighbourhood[city].size(); i++) {
		for (int j = 0; j < neighbourhood[city].size(); j++) {
			if (i != j) {
				if (neighbourhood[city][i] == neighbourhood[city][j]) {
					return neighbourhood[city][i];
				}
			}
		}
	}
	return -1;
}

// find nearest city from neighbourhood (EX)
int TSP::findNearest(std::vector<std::vector<int>> &neighbourhood, int city) {

	int distance = 2147483647;
	int next;

	//cout << "Neighbourhood city " << city << " " << neighbourhood[city].size() << endl;

	for (int i = 0; i < neighbourhood[city].size(); i++) {
		if (problem->matrix[city][neighbourhood[city][i]] < distance) {
			distance = problem->matrix[city][neighbourhood[city][i]];
			next = neighbourhood[city][i];
		}
	}

	return next;
}

// remove city from unpicked (EX & OX) 
void TSP::removeFromUnpicked(std::vector<int> &unpicked, int city) {
	for (int i = 0; unpicked.size(); i++) {
		if (unpicked[i] == city) {
			unpicked.erase(unpicked.begin() + i);
			break;
		}
	}
}

// return random city from unpicked (EX)
int TSP::randCityFromUnpicked(std::vector<int> &unpicked) {
	int city = rand() % unpicked.size();
	return unpicked[city];
}

// remove city from neighbourhood (EX)
void TSP::removeFromNeighbourhood(std::vector<std::vector<int>> &neighbourhood, int city) {
	for (int i = 0; i < neighbourhood.size(); i++) {
		for (int j = 0; j < neighbourhood[i].size(); j++) {
			if (neighbourhood[i][j] == city) {
				neighbourhood[i].erase(neighbourhood[i].begin() + j);
				j--;
			}
		}
	}
}

// invert elements in random section
void TSP::inversionMutation(std::vector<int> &individual) {

	int sectionStart;
	int sectionEnd;

	do {
		sectionStart = rand() % problem->size;
		sectionEnd = rand() % problem->size;
	} while (sectionStart == sectionEnd);

	if (sectionStart > sectionEnd) {
		int temp = sectionStart;
		sectionStart = sectionEnd;
		sectionEnd = temp;
	}

	while (sectionStart < sectionEnd) {
		neighbourhoodSwap(individual, sectionStart, sectionEnd);
		sectionStart++;
		sectionEnd--;
	}

}

// insert (random) element before another (random) one
void TSP::insertionMutation(std::vector<int> &individual) {
	
	int indexFirst;
	int indexSecond;

	do {
		indexFirst = rand() % problem->size;
		indexSecond = rand() % problem->size;
	} while (indexFirst == indexSecond);

	neighbourhoodInsert(individual, indexFirst, indexSecond);

}

// swap two random elements
void TSP::transpositionMutation(std::vector<int> &individual) {
	
	int indexFirst;
	int indexSecond;
	
	do {
		indexFirst = rand() % problem->size;
		indexSecond = rand() % problem->size;
	} while (indexFirst == indexSecond);

	neighbourhoodSwap(individual, indexFirst, indexSecond);

}