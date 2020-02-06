#include "pch.h"
#include "Ant.h"
#include <iostream>

using namespace std;

Ant::Ant(int** map, double** pheromoneMap, int numOfCities)
{
	this->map = map;
	this->pheromoneMap = pheromoneMap;
	this->numOfCities = numOfCities;
}


Ant::~Ant()
{
}

void Ant::setStartCity(int city) {
	startCity = city;
	currentCity = startCity;
	visited.push_back(startCity);
}

void Ant::run(double alfa, double beta) {

	vector<double> probability;

	while (visited.size() != numOfCities) {
		
		// count probability for each city

		// P_ij = (T_ij^alfa * N_ij^beta) / ( Sum of not visited (T_ij^alfa * N_ij^beta) )

		// T_ij - pheromone on edge ij
		// N_ij - distance on edge ij

		if (currentCity >= numOfCities) {
			cout << "CURRENT CITY = " << currentCity << endl;
			showPath();
			showPheromoneMap();
			showMap();
			break;
		}

		for (int i = 0; i < numOfCities; i++) {
			if (currentCity != i && notVisited(i)) {
				double N_ij;
				// WARNING! in data43 and data443 distance is equal 0 between several cities
				if (map[currentCity][i] == 0) {
					N_ij = 1.0 / (double)(1.0);
				}
				else {
					N_ij = 1.0 / (double)(map[currentCity][i]);
				}
				double T_ij = (double)pheromoneMap[currentCity][i];
				probability.push_back(pow(T_ij, alfa) * pow(N_ij, beta));
			}
			else {
				probability.push_back(0);
			}
		}

		double sum = 0.0;

		for (int i = 0; i < numOfCities; i++) {
			if (probability[i] != 0) {
				sum += probability[i];
			}
		}

		for (int i = 0; i < numOfCities; i++) {
			if (probability[i] != 0) {
				probability[i] /= sum;
			}
		}
		
		int next;
		
		next = roulette(probability);

		if(next == -1){
			next = randFromNotVisited();
		} 
		
		// choose city with highest probability
		// double best = -1;

		//for (int i = 0; i < numOfCities; i++) {
		//	if (probability[i] > best) {
		//		next = i;
		//		best = probability[i];
		//	}
		//}

		// set new current city
		currentCity = next;

		// add to visited
		visited.push_back(next);
		// clear probability
		probability.clear();
		probability.shrink_to_fit();
	}
}

int Ant::roulette(std::vector<double> prob) {

	double random = rand() / double(RAND_MAX);

	double sum = 0;

	for (int i = 0; i < prob.size(); i++) {
		sum += prob[i];
		prob[i] = sum;
	}

	for (int i = 0; i < prob.size(); i++) {
		if (random <= prob[i]) {
			return i;
		}
	}

	return -1;

}

int Ant::randFromNotVisited() {
	for (int i = 0; i < numOfCities; i++) {
		if (notVisited(i)) {
			return i;
		} 
	}
}

void Ant::clearVisited() {
	visited.clear();
}

bool Ant::notVisited(int nextCity) {

	for (int city : visited) {
		if (city == nextCity) return false;
	}

	return true;
}

void Ant::showMap() {
	cout << "Map : \n";
	for (int i = 0; i < numOfCities; i++) {
		for (int j = 0; j < numOfCities; j++) {
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
}
void Ant::showPheromoneMap() {
	cout << "Pheromone Map : \n";
	for (int i = 0; i < numOfCities; i++) {
		for (int j = 0; j < numOfCities; j++) {
			if (pheromoneMap[i][j] > 0.0001) {
				cout << pheromoneMap[i][j] << " ";
			}
			else {
				// easier to read from terminal
				cout << "0.00001" << " ";
			}
			
		}
		cout << endl;
	}
}
void Ant::showPath() {
	cout << "Path : ";
	for (int i = 0; i < visited.size(); i++) {
		cout << visited[i] << "->";
	}
	cout << endl;
}

std::vector<int> Ant::getPath() {
	return visited;
}