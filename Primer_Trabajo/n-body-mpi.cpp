#include <mpi.h>
#include <iostream>
#include <stdio.h>

using namespace std;

main(int argc, char **argv)
{
	int my_id, root_process, ierr, num_procs;

	MPI_Status status;

	ierr = MPI_Init(&argc, &argv);

	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	if( my_id == 0 ) {
		cout<<"Hola mundo soy el 0"<<endl;
	}
	else if( my_id == 1 ) {
		cout<<"Hola mundo soy el 1"<<endl;
	}
	else if( my_id == 2 ) {
		cout<<"Hola mundo soy el 2"<<endl;
	} 
	else {
		cout<<"Hola mundo soy cualquiera"<<endl;
	}

	ierr = MPI_Finalize();
}
