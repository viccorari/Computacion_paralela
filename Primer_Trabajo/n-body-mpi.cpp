#include <mpi.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

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

double randf(int LO, int HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

main(int argc, char **argv)
{

	double ** forces;
	double ** pos;
	double ** vel;
	double ** ace;
	double * masses;
	long n;
	long dim;
	double ** old_pos;
	double ** old_vel;
	
	n = 10;
	dim = 2;
	forces = init_mat(n, dim);
	pos = init_mat(n, dim);
	vel = init_mat(n, dim);
	ace = init_mat(n, dim);
	
	old_pos = init_mat(n, dim);
	old_vel = init_mat(n, dim);
	masses = new double[n];
	
	for (int i = 0; i < n; i++) {
		masses[i] = randf(0.1, 10.0);
		pos[i][0] = randf(0.001, 2.0);
		pos[i][1] = randf(0.001, 2.0);
	}

	int my_id, root_process, ierr, num_procs;

	MPI_Status status;

	ierr = MPI_Init(&argc, &argv);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	if( my_id == 0 ) {
		 
	}
	else if( my_id == 1 ) {
		 
	}
	else if( my_id == 2 ) {
		 
	}
	else {
		 
	}
	ierr = MPI_Finalize();
	

	
}
