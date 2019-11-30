#include "pch.h"
#include "PathStep.h"
#include <iostream>

using namespace std;


PathStep::PathStep(int x, int y, int cadence)
{
	this->x = x;
	this->y = y;
	this->cadence = cadence;
}


PathStep::~PathStep()
{
}

void PathStep::decreaseCadence()
{
	cadence--;
}

int PathStep::getX()
{
	return x;
}

int PathStep::getY()
{
	return y;
}

int PathStep::getCadence() {
	return cadence;
}

void PathStep::show()
{
	cout << "X:" << x << " Y:" << y << " Cadence: " << cadence << endl;
}
