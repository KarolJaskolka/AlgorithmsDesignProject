// App.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <string>
#include "Matrix.h"
#include "Timer.h"
#include "TSP.h"

using namespace std;

int main()
{
	string files[] = { "data10.txt", "data11.txt", "data12.txt", "data13.txt",
						"data14.txt", "data15.txt", "data16.txt", "data18.txt" };

	Timer *timer = new Timer();

	int bestPath;

	for (int i = 0; i < 8; i++) {

		TSP *tsp = new TSP(files[i]);
		
		//timer->start();
		//bestPath = tsp->nearestNeighbour(0);
		//timer->stop();
		//cout << "--------------------------------------------------------------------------------------------------\n";
		//cout << "Time : " << timer->result() << " ms | ";
		//cout << "Nearest Neighbour [0] : " << bestPath << endl;
		//tsp->showTempPath();

		timer->start();
		bestPath = tsp->kNearestNeighbour();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "K Nearest Neighbour : " << bestPath << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->BranchAndBound();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Branch And Bound : " << bestPath << endl;
		tsp->showBestPath();

		/*

		timer->start();
		bestPath = tsp->bruteForceSTL();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Brute Force (STL) : " << bestPath << endl;
		tsp->showBestPath();
		
		timer->start();
		bestPath = tsp->bruteForceSwap();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Brute Force (Swap): " << bestPath << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->bruteForceTreeFaster();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Brute Force (Tree Faster): " << bestPath << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->bruteForceTree();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Brute Force (Tree): " << bestPath << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->BranchAndBoundPenalty();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Branch And Bound Penalty : " << bestPath << endl;
		tsp->showPointPath();

		timer->start();
		bestPath = tsp->HeldKarp();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Held Karp bestPath : " << bestPath << endl;
		
		*/

		cout << "--------------------------------------------------------------------------------------------------\n";

		delete tsp;
		
	}


	return 0;
}

