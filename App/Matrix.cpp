#include "pch.h"
#include "Matrix.h"
#include "Point.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

// constructor
Matrix::Matrix()
{
	matrix = NULL;
	size = 0;
}

// constructor with size ( n x n )
Matrix::Matrix(int n) {
	
	size = n;
	
	matrix = new int*[size];
	
	for (int i = 0; i < size; i++) {
		
		matrix[i] = new int[size];

	}

}

// read from file [ format : name \n size \n matrix n x n ]
Matrix::Matrix(std::string fileName)
{
	fstream file;
	file.open(fileName.c_str(), ios::in);

	if (!file.good()) { return; }

	string name;
	file >> name;
	file >> size;

	cout << "\nFile name : " << name << ".txt, Size : " << size;

	matrix = new int*[size];

	for (int i = 0; i < size; i++) {
		matrix[i] = new int[size];
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			file >> matrix[i][j];
		}
	}

	file.close();
}

// copy constructor
Matrix::Matrix(Matrix * object)
{
	size = object->size;
	path = object->path;

	matrix = new int*[size];

	for (int i = 0; i < size; i++) {
		
		matrix[i] = new int[size];

		for (int j = 0; j < size; j++) {

			matrix[i][j] = object->matrix[i][j];

		}

	}

}

// destructor
Matrix::~Matrix()
{
	for (int i = 0; i < size; i++) {
		delete matrix[i];
	}
	delete matrix;
}

// generate matrix n x n with random numbers [10,99]
void Matrix::generate(int n)
{
	size = n;
	

	matrix = new int*[size];

	for (int i = 0; i < size; i++) {
		matrix[i] = new int[size];
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i != j) {
				matrix[i][j] = rand() % 90 + 10;

			}
			else {
				matrix[i][j] = -1;
			}
		}
	}
}

// display matrix
void Matrix::show()
{
	cout << "-----------------------------------------\n";
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << "-----------------------------------------\n";
}

// display path (store path for every solution during BnB)
void Matrix::showPath() {
	for (int i = 0; i < path.size(); i++) {
		cout << path[i] << " -> ";
	}
	cout << path[0] << endl;
}

// reduce matrix and return Lower Bound
int Matrix::reduceMatrix() {
	
	int lower = 0;
	int min;

	// Rows

	for (int i = 0; i < size; i++) {

		min = 2147483647; // max integer value

		// find row minimum
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] != -1 && matrix[i][j] < min) {
				min = matrix[i][j];
			}
		}

		// reduce row
		if (min > 0 && min != 2147483647) {
			for (int j = 0; j < size; j++) {
				if (matrix[i][j] != -1) {
					matrix[i][j] -= min;
				}
			}
			lower += min;
		}

	}

	// Columns

	for (int i = 0; i < size; i++) {

		min = 2147483647; // max integer value

		// find column minimum
		for (int j = 0; j < size; j++) {
			if (matrix[j][i] != -1 && matrix[j][i] < min) {
				min = matrix[j][i];
			}
		}

		// reduce column
		if (min > 0 && min != 2147483647) {
			for (int j = 0; j < size; j++) {
				if (matrix[j][i] != -1) {
					matrix[j][i] -= min;
				}
			}
			lower += min;
		}
	}

	bound = lower;

	return lower;
}

// prepare matrix to reduce and reduceMatrix(); return bound = lowerBound + cost[row][col] + reduceCost
int Matrix::reduceMatrix(int row, int col, int lowerBound) {

	path.push_back(col);

	int cost = matrix[row][col];

	matrix[col][row] = -1;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == row || j == col) {
				matrix[i][j] = -1;
			}
		}
	}

	int reduceCost = reduceMatrix();

	bound = lowerBound + cost + reduceCost;

	return bound;
}

// count all penalties and find maximum (BnB penalty)
void Matrix::countAndFindPenalty() {
	
	Matrix *penaltyMatrix = new Matrix(size);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] != 0) {
				penaltyMatrix->matrix[i][j] = -1;
			}
			else {
				int minRow = minRowValue(i, j);
				int minCol = minColValue(j, i);
				penaltyMatrix->matrix[i][j] = minRow + minCol;
			}
		}
	}


	int max = -1;
	Point *point = new Point();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (penaltyMatrix->matrix[i][j] >= max) {
				max = penaltyMatrix->matrix[i][j];
				point->setX(i);
				point->setY(j);
			}
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == point->getX() || j == point->getY()) {
				matrix[i][j] = -1;
			}
		}
	}
	matrix[point->getY()][point->getX()] = -1;
	pathPoint.push_back(point);

	delete penaltyMatrix;

}

// find minimum value in row
int Matrix::minRowValue(int row, int zeroPosition) {
	int min = 2147483647;
	for (int i = 0; i < size; i++) {
		int value = matrix[row][i];
		if (value != -1 && value < min && i != zeroPosition) {
			min = value;
		}
	}
	return min == 2147483647 ? 0 : min;
}

// find minimum value in column
int Matrix::minColValue(int col, int zeroPosition) {
	int min = 2147483647;
	for (int i = 0; i < size; i++) {
		int value = matrix[i][col];
		if (value != -1 && value < min && i != zeroPosition) {
			min = value;
		}
	}
	return min == 2147483647 ? 0 : min;
}

// return last city from path
int Matrix::getLastCity() {
	if (path.size() == 0) return 0;
	return path[path.size() - 1];
}