#include "pch.h"
#include "TabuList.h"
#include <iostream>
using namespace std;

// constructor
TabuList::TabuList(int size, int cadence)
{
	this->cadence = cadence;
	this->size = size;
}

// destructor
TabuList::~TabuList()
{
	tabu.clear();
	tabu.shrink_to_fit();
}

// decrease cadence of each element tabu list
void TabuList::decreaseAll()
{
	for (int i = 0; i < tabu.size(); i++) {
		tabu[i]->decreaseCadence();
		if (tabu[i]->getCadence() == 0) {
			delete tabu[i];
			tabu[i] = NULL;
			tabu.erase(tabu.begin() + i);
			i--;
		}
	}
}

// return position of element (if not found return -1)
int TabuList::find(int x, int y)
{	
	for (int i = 0; i < tabu.size(); i++) {
		if (tabu[i]->getX() == x && tabu[i]->getY() == y) {
			return i;
		}
		else if (tabu[i]->getY() == x && tabu[i]->getX() == y) {
			return i;
		}
	}
	return -1;
}

// add new step to tabu list (remove first element if max size reached)
void TabuList::add(int x, int y)
{
	if (tabu.size() == size) {
		removeFirst();
	}
	tabu.push_back(new PathStep(x, y, cadence));
}

// display tabu list
void TabuList::show()
{
	cout << "TABU LIST: \n";
	for (PathStep* step : tabu) {
		step->show();
	}
	cout << "-----------\n";
}

// remove first element from tabu list (with lowest cadence)
void TabuList::removeFirst()
{
	removeAtIndex(0);
}

// remove element from list at providen index
void TabuList::removeAtIndex(int index) {
	delete tabu[index];
	tabu[index] = NULL;
	tabu.erase(tabu.begin() + index);
}

// returns current size of tabu list
int TabuList::getCurrentSize()
{
	return tabu.size();
}

