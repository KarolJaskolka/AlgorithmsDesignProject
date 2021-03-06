#pragma once
#include "Matrix.h"
#include "PathStep.h"
class TabuList
{
private:
	std::vector<PathStep*> tabu;
	int cadence;
	int size;

public:
	TabuList(int size, int cadence);
	~TabuList();

	void decreaseAll();
	bool find(int x, int y);
	bool findAndRemove(int x, int y);
	void add(int x, int y);
	void show();
	void removeFirst();
	void removeAtIndex(int index);
	int getCurrentSize();
	void clear();
};

