// App.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <string>
#include "Matrix.h"
#include "TabuList.h"
#include "Timer.h"
#include "TSP.h"

using namespace std;

double error(int best, int found) {
	return ((double)(found - best) / (double)best) * 100;
}

int main()
{
	srand((unsigned int)time(0));

	string files[] = { "data10.txt", "data11.txt", "data12.txt", "data13.txt", 
						"data14.txt", "data15.txt", "data16.txt", "data18.txt",
						"data21.txt", "data24.txt", "data26.txt", "data29.txt",
						"data34.txt", "data36.txt", "data39.txt", "data42.txt",
						"data43.txt", "data45.txt", "data48.txt", "data53.txt",
						"data56.txt", "data58.txt", "data65.txt", "data70.txt",
						"data71.txt", "data100.txt", "data120.txt", "data171.txt",
						"data323.txt", "data358.txt", "data403.txt", "data443.txt" };

	// best known solution
	int best[] = { 212,202,264,269,125,291,156,187,2707,1272,937,1610,1286,
					1473,1530,699,5620,1613,14422,6905,1608,25395,1839,
						38673,1950,36230,6942,2755,1326,1163,2465,2720};

	Timer *timer = new Timer();
	
	int bestPath;

	int problems = 32; // number of problems from files[] to solve

	for (int i = 0; i < 10; i++) {

		TSP *tsp = new TSP(files[i]);
		cout << " Best: " << best[i] << endl;

		timer->start();
		bestPath = tsp->kNearestNeighbour();
		timer->stop();
		cout << "Time : " << timer->result() << " ms | ";
		cout << "K Nearest Neighbour : " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;

		
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				for (int l = 0; l < 2; l++) {
					
					int iterations = 1000;
					bool swap, diversification = false, aspiration = false, random = false;
					int tabuSize[] = { 2, 3, 4, 5, 6, 7, 8, 10, 14, 18, 20 };

					if (j % 2 == 0) { swap = true; } else { swap = false; }
					if (k % 2 == 0) { aspiration = false; } else { aspiration = true; }
					if (l % 2 == 0) { diversification = false; }else { diversification = true; }

					for (int t = 0; t < 11; t++) {
						timer->start();
						bestPath = tsp->TabuSearch(iterations, tabuSize[t], tabuSize[t]+1, swap, diversification, random, aspiration);
						timer->stop();
						cout << "Time : " << timer->result() << " ms | ";
						cout << "TS :" << " Iter " << iterations << " TSize " << tabuSize[t] << " Cad " << tabuSize[t]
							<< " Swap " << swap << " Div " << diversification << " Rand " << random << " Asp " << aspiration <<
							" Best " << bestPath << " Error " << error(best[i], bestPath) << " %" << endl;
					}
				}
			}
		}
		
		/*

		timer->start();
		bestPath = tsp->BranchAndBound(true);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Branch And Bound (kNN) : " << bestPath << endl;
		tsp->showBestPath();

		timer->start();
		bestPath = tsp->BranchAndBound(false);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Branch And Bound (NN) : " << bestPath << endl;
		tsp->showBestPath();
		
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
		*/
		cout << "--------------------------------------------------------------------------------------------------\n";


		delete tsp;
		
	}
	
	return 0;
}

