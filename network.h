#pragma once
#include "matrix.h"
#include "Header.h"
#include <fstream>

class network
{
	int lays;
	int* size;
	matrix* weight;
	double** bios;
	double** neuron_value;
	double** neuron_error;
	double* neurons_bios_value;
public:
	void init(data config)
	{
		this->lays = config.lays;
		this->size = new int[lays];
		for (int i = 0; i < lays; i++)
		{
			size[i] = config.size[i];
		}

		
		weight = new matrix[lays - 1];
		
		bios = new double* [lays - 1];
		for (int i = 0; i < (lays - 1); i++)
		{
			weight[i].init(size[i + 1], size[i]);
			bios[i] = new double[size[i+1]];
			for (int j = 0; j < size[i+1]; j++)
			{
				bios[i][j] = (rand() % 50 * 0.06 / (17 + j));
			}
		}

		neuron_value = new double* [lays];
		neuron_error = new double* [lays];
		for (int i = 0; i < lays; i++)
		{
			neuron_value[i] = new double[size[i]];
			neuron_error[i] = new double[size[i]];
		}

		neurons_bios_value = new double[lays - 1];

		for (int i = 0; i < (lays - 1); i++)
		{
			neurons_bios_value[i] = 1;
		}
		
	}

	void setinput(double* values)
	{
		for (int i = 0; i < size[0]; i++)
		{
			neuron_value[0][i] = values[i];
		}
	}

	double forwarfeed()
	{
		for (int i = 1; i < lays; i++)
		{
			matrix::multi(weight[i-1],neuron_value[i-1],size[i],neuron_value[i]);
			matrix::vectorsumm(neuron_value[i - 1],bios[i-1],size[i]);
			use(neuron_value[i],size[i]);
		}
		int pred = search_max_index(neuron_value[lays-1]);
		return pred;
	}

	int search_max_index(double* value)
	{
		double max = value[0];
		int prediction = 0;
		double tmp;
		for (int j = 1; j < size[lays - 1]; j++) {
			tmp = value[j];
			if (tmp > max) {
				prediction = j;
				max = tmp;
			}
		}
		return prediction;
	}

	void debug()
	{
		std::cout << " lays : " << lays << "\n *size[]: ";
		for (int i = 0; i < lays; i++)
		{
			std::cout << size[i] << " ";
		}
	}

	void backpropogation(double expect)
	{
		for (int i = 0; i < size[lays - 1]; i++)
		{
			if (i != int(expect))
				neuron_error[lays - 1][i] = -neuron_value[lays - 1][i] * useder(neuron_value[lays - 1][i]);	
			else
				neuron_error[lays - 1][i] = (1.0 - neuron_value[lays - 1][i]) * useder(neuron_value[lays - 1][i]);
		}
		for (int i = lays - 2; i > 0; i--)
		{
			matrix::multi_t(weight[i],neuron_error[i+1],size[i+1],neuron_error[i]);
			for (int j = 0; j < size[i]; j++)
			{
				neuron_error[i][j] *= useder(neuron_value[i][j]);
			}
		}
	}

	void weight_update(double learning_rate)
	{
		for (int i = 0; i < lays - 1; i++)
		{
			for (int j = 0; j < size[i + 1]; j++)
			{
				for (int k = 0; k < size[i]; k++)
				{
					weight[i](j, k) += neuron_value[i][k] * neuron_error[i + 1][j] * learning_rate;
				}
			}
		}
		for (int i = 0; i < lays - 1; i++)
		{
			for (int k = 0; k < size[i + 1]; k++)
			{
				bios[i][k] += neuron_error[i + 1][k] * learning_rate;
			}
		}
	}

	void saveweights()
	{
		std::ofstream fout;
		fout.open("weights.txt");
		if (!fout.is_open()) {
			std::cout << "Error reading the file";
			system("pause");
		}
		for (int i = 0; i < lays - 1; ++i)
			fout << weight[i] << " ";

		for (int i = 0; i < lays - 1; ++i) {
			for (int j = 0; j < size[i + 1]; ++j) {
				fout << bios[i][j] << " ";
			}
		}
		std::cout << "Weights saved \n";
		fout.close();
	}

	void readweights()
	{
		std::ifstream fin;
		fin.open("Weights.txt");
		if (!fin.is_open()) {
			std::cout << "Error reading the file";
			system("pause");
		}
		for (int i = 0; i < lays - 1; ++i) {
			fin >> weight[i];
		}
		for (int i = 0; i < lays - 1; ++i) {
			for (int j = 0; j < size[i + 1]; ++j) {
				fin >> bios[i][j];
			}
		}
		std::cout << "Weights readed \n";
		fin.close();
	}

	void memory_free()
	{
		weight->memory_free();
		delete size;
		delete weight;
		delete bios;
		delete neuron_error;
		delete neuron_value;
		delete neurons_bios_value;
	}

private:
	void use(double* value, int n)
	{
		for (int i = 0; i < n; i++)
		{
			if (value[i] < 0)
			{
				value[i] *= 0.01;
			}
			else if (value[i] > 1)
			{
				value[i] = 1.0 + 0.01 * (value[i] + 1.0);
			}
		}
	}


	void useder(double* value, int n)
	{
		for (int i = 0; i < n; i++)
		{
			if (value[i] < 0 || value[i] > 1)
			{
				value[i] = 0.01;
			}
			else
			{
				value[i] = 1;
			}
		}
	}
	double useder(double value)
	{
		if (value < 0 || value > 1)
		{
			value = 0.01;
		}
		else
		{
			value = 1;
		}
		return value;
	}
};