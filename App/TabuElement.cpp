#include "pch.h"
#include "TabuElement.h"
#include <iostream>

TabuElement::TabuElement(int city, int cadence)
{
	this->city = city;
	this->cadence = cadence;
}


TabuElement::~TabuElement()
{
}

void TabuElement::decreaseCadence()
{
	cadence--;
}

int TabuElement::getCity()
{
	return city;
}

int TabuElement::getCadence()
{
	return cadence;
}

void TabuElement::show()
{
	std::cout << "City:" << city << " Cadence: " << cadence << std::endl;
}
