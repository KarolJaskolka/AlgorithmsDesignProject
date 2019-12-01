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

// returns current size of tabu list
int TabuList::getCurrentSize()
{
	return tabu.size();
}

// decrease cadence of each element tabu list
void TabuList::decreaseAll()
{
	for (int i = 0; i < tabu.size(); i++) {
		tabu[i]->decreaseCadence();
		if (tabu[i]->getCadence() == 0) {
			removeAtIndex(i);
			i--;
		}
	}
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

/********************************************* PATH STEP ********************************************/

/*
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
*/

/******************************************* TABU ELEMENT **********************************************/

bool TabuList::findAndRemove(int x, int y) {
	bool found = false;
	for (int i = 0; i < tabu.size(); i++) {
		if (tabu[i]->getCity() == x) {
			removeAtIndex(i);
			found = true;
			i--;
		}
		else if (tabu[i]->getCity() == y) {
			removeAtIndex(i);
			found = true;
			i--;
		}
	}
	return found;
}

bool TabuList::find(int x, int y) {
	for (int i = 0; i < tabu.size(); i++) {
		if (tabu[i]->getCity() == x || tabu[i]->getCity() == y) {
			return true;
		}
	}
	return false;
}

void TabuList::add(int x, int y) {
	if (tabu.size() == size) {
		removeFirst();
	}
	
	tabu.push_back(new TabuElement(x, cadence));
	
	if (tabu.size() == size) {
		removeFirst();
	}

	tabu.push_back(new TabuElement(y, cadence));
}

void TabuList::show() {
	cout << "TABU LIST: \n";
	for (TabuElement* element : tabu) {
		element->show();
	}
	cout << "-----------\n";
}
