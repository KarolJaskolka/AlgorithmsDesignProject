#pragma once
class TabuElement
{
private:
	int city;
	int cadence;
public:
	TabuElement(int city, int cadence);
	~TabuElement();

	void decreaseCadence();
	int getCity();
	int getCadence();
	void show();
};

