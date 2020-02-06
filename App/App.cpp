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

double d_error(double best, double found) {
	return ((found - best)/best) * 100;
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

	string fileName;

	cout << "File name : ";
	cin >> fileName;

	int index = -1, bestPath;

	for (int i = 0; i < 33; i++) {
		if (fileName == files[i]) {
			index = i;
		}
	}
	
	if (index != -1) {
		
		double maxDuration;
		cout << "Max duration time [ms] : ";
		cin >> maxDuration;

		TSP *tsp = new TSP("Data/" + files[index]);

		cout << " Best: " << best[index] << endl;

		timer->start();
		bestPath = tsp->kNearestNeighbour();
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "K Nearest Neighbour : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->LocalSearch(1000000, true, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Local Search Rand : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->LocalSearch(1000000, false, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Local Search kNN : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->SimulatedAnnealing(true, 10000, 0.999, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Simulated Annealing Rand : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->SimulatedAnnealing(false, 10000, 0.999, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Simulated Annealing kNN : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->TabuSearch(1000000, 10, 11, false, true, true, false, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Tabu Search Rand Insert : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->TabuSearch(1000000, 10, 11, true, true, true, false, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Tabu Search Rand Swap : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->TabuSearch(1000000, 10, 11, false, true, false, false, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Tabu Search kNN Insert : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->TabuSearch(1000000, 10, 11, true, true, false, false, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Tabu Search kNN Swap : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->GeneticAlgorithm(100, 1000000, 0, 0, 1, 80, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Genetic Algorithm OX Invert : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->GeneticAlgorithm(100, 1000000, 0, 1, 1, 80, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Genetic Algorithm OX Insert : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->GeneticAlgorithm(100, 1000000, 0, 2, 1, 80, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Genetic Algorithm OX Swap : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->GeneticAlgorithm(100, 1000000, 1, 0, 1, 80, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Genetic Algorithm PMX Invert : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->GeneticAlgorithm(100, 1000000, 1, 1, 1, 80, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Genetic Algorithm PMX Insert : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->GeneticAlgorithm(100, 1000000, 1, 2, 1, 80, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Genetic Algorithm PMX Swap : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->AntColonyOptimization(1000000, 0, 0.5, 1, 2, 0.05, 0.1, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Ant Colony Optimization DAS : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->AntColonyOptimization(1000000, 1, 0.5, 1, 2, 0.05, 1, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Ant Colony Optimization QAS : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		timer->start();
		bestPath = tsp->AntColonyOptimization(1000000, 2, 0.5, 1, 2, 0.05, 10, maxDuration);
		timer->stop();
		cout << "--------------------------------------------------------------------------------------------------\n";
		cout << "Time : " << timer->result() << " ms | ";
		cout << "Ant Colony Optimization CAS : " << bestPath << " Error " << error(best[index], bestPath) << " %" << endl;
		//tsp->showBestPath();

		delete tsp;
	}
	else {
		cout << "File Not Found" << endl;
	}

	delete timer;

	return 0;
}

