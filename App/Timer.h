#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
private:
	high_resolution_clock::time_point begin;
	high_resolution_clock::time_point end;
public:
	Timer();
	~Timer();

	void start();
	void stop();
	double result();
};

