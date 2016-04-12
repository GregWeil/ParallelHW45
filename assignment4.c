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

int main(int argc, char *argv[]){
	
	int rowsize = atoi(argv[1]);
    int mpi_myrank;
    int mpi_commsize;
	
	//initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_myrank);
    InitDefault();//initialize RNG streams
    MPI_Barrier(MPI_COMM_WORLD);
	
	
	//initialize row data and send to other ranks
	int chunk_size = rowsize / mpi_commsize;
	float** matrix_rows = malloc(chunk_size * sizeof(float*));
	float** transpose_rows = malloc(chunk_size * sizeof(float*));
	MPI_Request request;
	MPI_Status status;
	
	//create and send row data
	int testval = mpi_myrank*rowsize*chunk_size;
	for(int i=0; i<chunk_size; i++){
		//allocate row
		matrix_rows[i] = malloc(rowsize * sizeof(float));
		transpose_rows[i] = malloc(rowsize * sizeof(float));
		for(int j=0; j<rowsize; j++){
			//matrix_rows[i][j] = GenVal(i*mpi_myrank);
			matrix_rows[i][j] = testval++;
		}		
		//send row data to the rank that needs it
		for(int j=0; j<mpi_commsize; j++){
			MPI_Isend(&(matrix_rows[i][j*chunk_size]), chunk_size, MPI_INT, j, mpi_myrank*i+i, MPI_COMM_WORLD, &request);
			MPI_Request_free(&request);
		}
	}
	
	//receive row data and construct transpose
	
	//temporary array for transpose construction
	float* tmp = malloc(chunk_size * sizeof(float));
	
	//from each rank, receive row_size/rank rows
	for(int j=0; j<mpi_commsize; j++){//for each rank
		for(int i=0; i<chunk_size; i++){//for each row
			MPI_Irecv(tmp, chunk_size, MPI_INT, j, j*i+i, MPI_COMM_WORLD, &request);
			MPI_Wait(&request, &status);

			for(int k=0;k<chunk_size;k++){
				//index of column in tmp == index of row in transpose
				transpose_rows[k][j*chunk_size + i] = tmp[k];
			}
		}	
	}
	
	free(tmp);
	
	//finalize
    MPI_Barrier(MPI_COMM_WORLD);
	
	/*print goes here*/
	
	//debug print
	for(int x=0; x<chunk_size; x++){
		printf("rank %d transpose row %d:\n",mpi_myrank,x);
		for(int k=0; k<rowsize;k++){
			printf("%.0f\t",transpose_rows[x][k]);
		}
		printf("\n");
	}
	
    MPI_Finalize();
	
	//free all dynamically allocated memory
	for(int i=0; i<chunk_size; i++){
		free(matrix_rows[i]);
		free(transpose_rows[i]);
	}
	free(matrix_rows);
	free(transpose_rows);
	
    return 0;
}