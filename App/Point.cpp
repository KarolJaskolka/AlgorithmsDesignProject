#include "pch.h"
#include "Point.h"


Point::Point()
{
	x = 0;
	y = 0;
}


Point::~Point()
{
}

void Point::setX(int number)
{
	x = number;
}

void Point::setY(int number)
{
	y = number;
}

int Point::getX()
{
	return x;
}

int Point::getY()
{
	return y;
}
