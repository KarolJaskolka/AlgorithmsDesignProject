// App.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <string>
#include "Timer.h"
#include "TSP.h"

using namespace std;

double error(int best, int found) {
	return ((double)(found - best) / (double)best) * 100;
}

int main()
{
	srand((unsigned int)time(0));

	string files[] = { "data10.txt", "data11.txt", "data12.txt", "data13.txt", "data14.txt", 
						"data15.txt", "data16.txt", "data17.txt", "data18.txt", "data21.txt", 
						"data24.txt", "data26.txt", "data29.txt", "data34.txt", "data36.txt", 
						"data39.txt", "data42.txt", "data43.txt", "data45.txt", "data48.txt", 
						"data53.txt", "data56.txt", "data58.txt", "data65.txt", "data70.txt",
						"data71.txt", "data100.txt", "data120.txt", "data171.txt", "data323.txt", 
						"data358.txt", "data403.txt", "data443.txt" };

	// best known solution
	int best[] = { 212,202,264,269,125,291,156,2085, 187,2707,1272,937,1610,1286,
					1473,1530,699,5620,1613,14422,6905,1608,25395,1839,
						38673,1950,36230,6942,2755,1326,1163,2465,2720};

	Timer *timer = new Timer();
	
	int bestPath;

	int problems = 33; // number of problems from files[] to solve

	for (int i = 0; i < 1; i++) {

		TSP *tsp = new TSP(files[i]);

		cout << " Best: " << best[i] << endl;

		tsp->GeneticAlgorithm(10);

		/*
		timer->start();
		bestPath = tsp->nearestNeighbour(0);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Nearest Neighbour [0] : " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
		tsp->showTempPath();

		timer->start();
		bestPath = tsp->kNearestNeighbour();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "K Nearest Neighbour : " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->LocalSearch(1000);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Local Search : " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->TabuSearch(1000, 10, 11, true, true, true, false);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Tabu Search : " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->BranchAndBound(true);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Branch And Bound (kNN) : " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->BranchAndBound(false);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Branch And Bound (NN) : " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
		tsp->showBestPath();
		
		timer->start();
		bestPath = tsp->bruteForceSTL();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Brute Force (STL) : " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->bruteForceSwap();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Brute Force (Swap): " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->bruteForceTreeFaster();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Brute Force (Tree): " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
		tsp->showBestPath();

		cout << "--------------------------------------------------------------------------------------------------\n";

		delete tsp;
		*/
	}
	
	return 0;
}

