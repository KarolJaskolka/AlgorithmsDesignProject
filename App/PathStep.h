#pragma once
class PathStep
{
private:
	int x;
	int y;
	int cadence;
public:
	PathStep(int x, int y, int cadence);
	~PathStep();

	void decreaseCadence();
	int getX();
	int getY();
	int getCadence();
	void show();
};

