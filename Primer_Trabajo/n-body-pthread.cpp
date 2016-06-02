#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <cstdlib>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

using namespace std;

class n_body;

struct arg_struct {
    public:
    n_body * b;
    int t_id;
};

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
	int t_long;
	int t;
	int tid;
	
	n_body(int n, int dim) {
		reset(n, dim);
	}

	void reset(int n, int dim) {
		t=0;
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
		print_d(ace, "Aceleracion:");
		print_d(forces, "Fuerza:");
	}


	void copy_data(double ** origen, double ** destino) {
		for (int i = 0; i < n_particulas; i++) {
			for (int j = 0; j < dimension; j++) {
				destino[i][j] = origen[i][j];
			}
		}
	}
	
	static void* n_part_f(void* p)
	{
		static_cast<n_body *>(static_cast<arg_struct *>(p)->b)->n_part(static_cast<arg_struct *>(p)->t_id);
		return NULL;
	}


	void n_part (int tid){
		double G = 6.673 * pow(10, -11);
		for (int q = tid*t_long ; q < (tid*t_long)+t_long; q++) {

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
	const int steps = 500000;
	n_body body(Tam, Dim);
	double masses[Tam];
	double pos_x[Tam];
	double pos_y[Tam];

	for (int i = 0; i < Tam; i++) {
		masses[i] = randf(1, 5);
		pos_x[i] = randf(0.001, 2);
		pos_y[i] = randf(0.001, 2);
	}

	cout << endl << "Parallel pthread" << endl;
	body.reset(Tam, Dim);
	body.input_data_2(pos_x, pos_y, masses);

	int n_t = 5;

	double G = 6.673 * pow(10, -11);

	pthread_t threads[n_t];
	int t_long = Tam/n_t;
	body.t_long = t_long;
	void *status;
	pthread_attr_t child_attr;
	pthread_attr_init(&child_attr); 
	pthread_attr_setdetachstate(&child_attr, PTHREAD_CREATE_DETACHED);

	for (int i = 0; i < steps; i++) {
		
		for(int it=0; it < n_t; it++ ){
			arg_struct ad;
			ad.b = &body;
			ad.t_id = it;
			int rc = pthread_create(&threads[it], &child_attr, n_body::n_part_f, &ad);
			if (rc){
				cout << "Error:unable to create thread, " << rc << endl;
				exit(-1);
			}
		}
	
		if(0){
			cout <<endl <<"Step:" << i << endl;
			body.print_data();
		}
		body.t += delta;
	}

	body.print_mat(body.forces);
	pthread_exit(NULL);
}



