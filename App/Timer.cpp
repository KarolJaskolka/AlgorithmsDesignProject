#include "pch.h"
#include "Timer.h"

Timer::Timer() {}

Timer::~Timer() {}

void Timer::start()
{
	begin = high_resolution_clock::now();
}

void Timer::stop()
{
	end = high_resolution_clock::now();
}

double Timer::result()
{
	return  (double)std::chrono::duration<double, std::milli>(end - begin).count();
}

double Timer::duration()
{
	return  (double)std::chrono::duration<double, std::milli>(high_resolution_clock::now() - begin).count();
}