#include <iostream>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

using namespace std;

class n_body {
public:

	double ** forces;
	double ** pos;
	double ** vel;
	double ** ace;
	double * masses;
	long n_particulas;
	long dimension;
	double ** old_pos;
	double ** old_vel;

	n_body(int n, int dim) {
		reset(n, dim);
	}

	void reset(int n, int dim) {
		n_particulas = n;
		dimension = dim;
		pos = init_mat(n, dim);
		vel = init_mat(n, dim);
		ace = init_mat(n, dim);
		forces = init_mat(n, dim);
		old_pos = init_mat(n, dim);
		old_vel = init_mat(n, dim);
	}

	void input_data_2(double * _pos_init_x, double * _pos_init_y, double * _masses) {
		for (int i = 0; i < n_particulas; i++) {
			pos[i][0] = _pos_init_x[i];
			pos[i][1] = _pos_init_y[i];
		}
		masses = _masses;
	}

	double ** & init_mat(int n, int dim) {
		double ** temp = new double*[n];
		for (int i = 0; i < n; i++) {
			temp[i] = new double[dim];
			for (int j = 0; j < dim; j++) {
				temp[i][j] = 0;
			}
		}
		return temp;
	}

	double *** & init_mat_d(int n, int dim) {
		double *** temp = new double**[n];
		for (int i = 0; i < n; i++) {
			temp[i] = new double *[i];
			for (int j = 0; j < n; j++) {
				temp[i][j] = new double[dim];
				for (int k = 0; k < dim; k++)
					temp[i][j][k] = 0;
			}
		}
		return temp;
	}

	void print_mat(double ** t) {
		for (int i = 0; i <n_particulas; i++) {
			for (int j = 0; j < dimension; j++) {
				cout << t[i][j] << " ";
			}
			cout << endl;
		}
	}

	void print_d(double ** m, char d[]) {
		cout << d << endl;
		for (int i = 0; i < n_particulas; i++) {
			for (int j = 0; j < dimension; j++) {
				cout << m[i][j];
				if (j != dimension - 1) cout << ",";
			}
			cout << "|";
		}
		cout << endl;
	}

	void print_data() {
		print_d(pos, "Posicion:");
		print_d(vel, "Velocidad:");
		print_d(ace, "Velocidad:");
		print_d(forces, "Fuerza:");
	}

	void lineal() {
		double G = 6.673 * pow(10, -11);
		for (int q = 0; q < n_particulas; q++) {
			for (int k = 0; k < n_particulas; k++) {
				if (k != q) {
					double x_diff = pos[q][0] - pos[k][0];
					double y_diff = pos[q][1] - pos[k][1];
					double dist = sqrtf(powf(x_diff, 2) + powf(y_diff, 2));
					double dist_cubed = powf(dist, 3);
					forces[q][0] -= ((G * masses[q] * masses[k]) / dist_cubed) * x_diff;
					forces[q][1] -= ((G * masses[q] * masses[k]) / dist_cubed) * y_diff;
				}
			}
		}
	}
};

double randf(int LO, int HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}


int main()
{
	srand(time(NULL));
	const int Tam = 10;
	const int Dim = 2;
	const float delta = 0.01;
	const int steps = 100;
	n_body body(Tam, Dim);
	double masses[Tam];
	double pos_x[Tam];
	double pos_y[Tam];

	for (int i = 0; i < Tam; i++) {
		masses[i] = randf(1, 5);
		pos_x[i] = randf(0.001, 2);
		pos_y[i] = randf(0.001, 2);
	}

	cout <<endl<< "Lineal" << endl;
	body.input_data_2(pos_x, pos_y, masses);
	body.lineal();
	body.print_mat(body.forces);
}
