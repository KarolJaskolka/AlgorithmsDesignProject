#pragma once

#include <string>
#include <vector>

class Matrix
{
public:

	int size;
	int **matrix;
	int bound;
	int id;

	std::vector<int> path;

	Matrix();
	Matrix(int n);
	Matrix(std::string fileName);
	Matrix(Matrix *object);
	
	~Matrix();

	// methods
	void generate(int n);
	void show();
	void showPath();
	int reduceMatrix();

	int reduceMatrix(int row, int col, int lowerBound);
	int minRowValue(int row, int zeroPosition);
	int minColValue(int col, int zeroPosition);
	int getLastCity();
	
};

