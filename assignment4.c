/***************************************************************************/
/* Asssignment 4 ***********************************************************/
/* Craig Carlson ***********************************************************/
/* Richard Pietrzak ********************************************************/
/* Greg Weil ***************************************************************/
/***************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<clcg4.h>
#include<mpi.h>


/*First, each MPI rank will initial it’s local slice of the matrix. The smallest number of BG/Q nodes that
can be used is 32. Here, each node will have a a 4 GB slice of matrix. Also, the transpose will consume
another 4 GB. Those slices could be subdivided further if there are multiple MPI ranks per node or not if
pthreads are used and there is only 1 MPI rank per BG/Q node.
*/

int main(int argc, char *argv[]){
	
    int mpi_myrank;
    int mpi_commsize;
	
	//initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_myrank);
    InitDefault();//initialize RNG streams
    MPI_Barrier(MPI_COMM_WORLD);
	
	/*
		DO COMPUTATION
	*/
	
	//finalize
    MPI_Barrier(MPI_COMM_WORLD);
	
	/*
		DO OUTPUT
	*/
	
    MPI_Finalize();
    return 0;
}