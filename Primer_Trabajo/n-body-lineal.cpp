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

	void calc1() {
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

	void calc2() {
		double G = 6.673 * pow(10, -11);
		for (int q = 0; q < n_particulas; q++) {
			for (int k = 0; k < n_particulas; k++) {
				if (k > q) {
					double x_diff = pos[q][0] - pos[k][0];
					double y_diff = pos[q][1] - pos[k][1];
					double dist = sqrtf(powf(x_diff, 2) + powf(y_diff, 2));
					double dist_cubed = powf(dist, 3);
					double forceqkx = ((G * masses[q] * masses[k]) / dist_cubed) * x_diff;
					double forceqky = ((G * masses[q] * masses[k]) / dist_cubed) * y_diff;
					forces[q][0] += forceqkx;
					forces[q][1] += forceqky;
					forces[k][0] -= forceqkx;
					forces[k][1] -= forceqky;
				}
			}
		}
	}

	void copy_data(double ** origen, double ** destino) {
		for (int i = 0; i < n_particulas; i++) {
			for (int j = 0; j < dimension; j++) {
				destino[i][j] = origen[i][j];
			}
		}
	}

	void calc3(double delta, int steps) {
		double G = 6.673 * pow(10, -11);
		double t = 0;
		for (int i = 0; i < steps; i++) {
			for (int q = 0; q < n_particulas; q++) {

				copy_data(pos, old_pos);
				copy_data(vel, old_vel);

				for (int k = 0; k < n_particulas; k++) {
					if (k > q) {
						double x_diff = old_pos[q][0] - old_pos[k][0];
						double y_diff = old_pos[q][1] - old_pos[k][1];
						double dist = sqrtf(powf(x_diff, 2) + powf(y_diff, 2));
						double dist_cubed = powf(dist, 3);
						double forceqkx = ((G * masses[q] * masses[k]) / dist_cubed) * x_diff;
						double forceqky = ((G * masses[q] * masses[k]) / dist_cubed) * y_diff;
						forces[q][0] += forceqkx;
						forces[q][1] += forceqky;
						forces[k][0] -= forceqkx;
						forces[k][1] -= forceqky;
					}
				}
				ace[q][0] = forces[q][0] / masses[q];
				ace[q][1] = forces[q][1] / masses[q];
				pos[q][0] += t*old_vel[q][0];
				pos[q][1] += t*old_vel[q][1];
				vel[q][0] += t / masses[q] * forces[q][0];
				vel[q][1] += t / masses[q] * forces[q][1];
			}
			/*
			cout <<endl <<"Step:" << i << endl;
			print_data();
			*/
			t += delta;
		}
	}

	void calc4(double delta, int steps) {
		double G = 6.673 * pow(10, -11);
		double t = 0;

		for (int i = 0; i < steps; i++) {
#pragma omp parallel
			{
#pragma omp for
				for (int q = 0; q < n_particulas; q++) {

					copy_data(pos, old_pos);
					copy_data(vel, old_vel);

					for (int k = 0; k < n_particulas; k++) {
						if (k > q) {
							double x_diff = old_pos[q][0] - old_pos[k][0];
							double y_diff = old_pos[q][1] - old_pos[k][1];
							double dist = sqrtf(powf(x_diff, 2) + powf(y_diff, 2));
							double dist_cubed = powf(dist, 3);
							double forceqkx = ((G * masses[q] * masses[k]) / dist_cubed) * x_diff;
							double forceqky = ((G * masses[q] * masses[k]) / dist_cubed) * y_diff;
							forces[q][0] += forceqkx;
							forces[q][1] += forceqky;
							forces[k][0] -= forceqkx;
							forces[k][1] -= forceqky;
						}
					}

				}

#pragma omp for
				for (int q = 0; q < n_particulas; q++) {
					ace[q][0] = forces[q][0] / masses[q];
					ace[q][1] = forces[q][1] / masses[q];
					pos[q][0] += t*old_vel[q][0];
					pos[q][1] += t*old_vel[q][1];
					vel[q][0] += t / masses[q] * forces[q][0];
					vel[q][1] += t / masses[q] * forces[q][1];
				}


				/*
				cout <<endl <<"Step:" << i << endl;
				print_data();
				*/
				t += delta;
			}
		}
	}

	void calc5(double delta, int steps) {
		double G = 6.673 * pow(10, -11);
		double t = 0;

		for (int i = 0; i < steps; i++) {
#pragma omp parallel
			{
#pragma omp for
				for (int q = 0; q < n_particulas; q++) {

					copy_data(pos, old_pos);
					copy_data(vel, old_vel);

					for (int k = 0; k < n_particulas; k++) {
						if (k > q) {
							double x_diff = old_pos[q][0] - old_pos[k][0];
							double y_diff = old_pos[q][1] - old_pos[k][1];
							double dist = sqrtf(powf(x_diff, 2) + powf(y_diff, 2));
							double dist_cubed = powf(dist, 3);
							double forceqkx = ((G * masses[q] * masses[k]) / dist_cubed) * x_diff;
							double forceqky = ((G * masses[q] * masses[k]) / dist_cubed) * y_diff;
							forces[q][0] += forceqkx;
							forces[q][1] += forceqky;
							forces[k][0] -= forceqkx;
							forces[k][1] -= forceqky;
						}
					}
					ace[q][0] = forces[q][0] / masses[q];
					ace[q][1] = forces[q][1] / masses[q];
					pos[q][0] += t*old_vel[q][0];
					pos[q][1] += t*old_vel[q][1];
					vel[q][0] += t / masses[q] * forces[q][0];
					vel[q][1] += t / masses[q] * forces[q][1];
				}

				/*
				cout <<endl <<"Step:" << i << endl;
				print_data();
				*/
				t += delta;
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
	n_body body(Tam, Dim);
	double masses[Tam];
	double pos_x[Tam];
	double pos_y[Tam];

	for (int i = 0; i < Tam; i++) {
		masses[i] = randf(1, 5);
		pos_x[i] = randf(0.001, 2);
		pos_y[i] = randf(0.001, 2);
	}
	/*
	cout <<endl<< "1er code" << endl;
	body.input_data_2(pos_x, pos_y, masses);
	body.calc1();
	body.print_mat(body.forces);
	cout <<endl<< "2do code" << endl;
	body.reset(Tam, Dim);
	body.input_data_2(pos_x, pos_y, masses);
	body.calc2();
	body.print_mat(body.forces);
	*/
	cout << endl << "Parallel code1" << endl;
	body.reset(Tam, Dim);
	body.input_data_2(pos_x, pos_y, masses);
	double start = omp_get_wtime();
	body.calc4(0.5, 500000);
	double end = omp_get_wtime();

	double wtick = omp_get_wtick();

	printf("start = %.16g\nend = %.16g\ndiff = %.16g\n",
		start, end, end - start);

	printf("wtick = %.16g\n1/wtick = %.16g\n",
		wtick, 1.0 / wtick);

	body.print_mat(body.forces);


	cout << endl << "Parallel code2" << endl;
	body.reset(Tam, Dim);
	body.input_data_2(pos_x, pos_y, masses);
	start = omp_get_wtime();
	body.calc5(0.5, 500000);
	end = omp_get_wtime();

	wtick = omp_get_wtick();

	printf("start = %.16g\nend = %.16g\ndiff = %.16g\n",
		start, end, end - start);

	printf("wtick = %.16g\n1/wtick = %.16g\n",
		wtick, 1.0 / wtick);

	body.print_mat(body.forces);

	system("PAUSE");
	return 0;
}
