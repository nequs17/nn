#pragma once
#include <iostream>
#include "Header.h"

class matrix
{
	double** weight;
	int row;
	int col;

public:
	void init(int row, int col)
	{
		this->row = row;
		this->col = col;

		weight = new double* [row];

		for (int i = 0; i < row; i++)
		{
			weight[i] = new double[col];
			for (int j = 0; j < col; j++)
			{
				weight[i][j] = (rand() % 50 * 0.03 / (17 + j));
			}
		}
	}

	static void multi(const matrix& m, const double* neuron, int n, double* c)
	{
		for (int i = 0; i < m.row; i++)
		{
			double temp = 0;
			for (int j = 0; j < m.col; j++)
			{
				temp += m.weight[i][j] * neuron[j];
			}
			c[i] = temp;
		}
	}

	static void multi_t(const matrix& m, const double* neuron, int n, double* c)
	{
		for (int i = 0; i < m.col; i++)
		{
			double temp = 0;
			for (int j = 0; j < m.row; j++)
			{
				temp += m.weight[j][i] * neuron[j];
			}
			c[i] = temp;
		}
	}

	static void vectorsumm(double* a, const double* b, int n)
	{
		for (int i = 0; i < n; i++)
		{
			a[i] += b[i];
		}
	}

	void debug()
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				std::cout << weight[i][j] << " ";
			}
			std::cout << "\n";
		}
	}

	double& operator()(int i, int j)
	{
		return weight[i][j];
	}

	friend std::ostream& operator << (std::ostream& os, const matrix& m)
	{
		for (int i = 0; i < m.row; ++i) {
			for (int j = 0; j < m.col; j++) {
				os << m.weight[i][j] << " ";
			}
		}
		return os;
	}
	friend std::istream& operator >> (std::istream& is, matrix& m)
	{
		for (int i = 0; i < m.row; ++i) {
			for (int j = 0; j < m.col; j++) {
				is >> m.weight[i][j];
			}
		}
		return is;
	}

	void memory_free()
	{
		delete weight;
	}

private:
	void _memory_free()
	{
		delete[] weight;
	}

};