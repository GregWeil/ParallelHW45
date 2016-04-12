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


/* test data */
float matrix[9][9]  = {
 {0,  1,  2,  3,  4,  5,  6,  7,  8 },
 {9,  10, 11, 12, 13, 14, 15, 16, 17},
 {18, 19, 20, 21, 22, 23, 24, 25, 26},
 {27, 28, 29, 30, 31, 32, 33, 34, 35},
 {36, 37, 38, 39, 40, 41, 42, 43, 44},
 {45, 46, 47, 48, 49, 50, 51, 52, 53},
 {54, 55, 56, 57, 58, 59, 60, 61, 62},
 {63, 64, 65, 66, 67, 68, 69, 70, 71},
 {72, 73, 74, 75, 76, 77, 78, 79, 80}};
 
float transpose[9][9] = {
 {0, 9,  18, 27, 36, 45, 54, 63, 72},
 {1, 10, 19, 28, 37, 46, 55, 64, 73},
 {2, 11, 20, 29, 38, 47, 56, 65, 74},
 {3, 12, 21, 30, 39, 48, 57, 66, 75},
 {4, 13, 22, 31, 40, 49, 58, 67, 76},
 {5, 14, 23, 32, 41, 50, 59, 68, 77},
 {6, 15, 24, 33, 42, 51, 60, 69, 78},
 {7, 16, 25, 34, 43, 52, 61, 70, 79},
 {8, 17, 26, 35, 44, 53, 62, 71, 80}};


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
	float** matrix_rows = malloc(chunk_size * sizeof(float));
	float** transpose_rows = malloc(chunk_size * sizeof(float));
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
			
			//printf("tmpchunk from rank %d: %.0f %.0f %.0f\n",j,tmp[0],tmp[1],tmp[2]);

			for(int k=0;k<chunk_size;k++){
				//index of column in tmp == index of row in transpose
				transpose_rows[k][j*chunk_size + i] = tmp[k];
			}
		}	
	}
	
	free(tmp);
	
	//finalize
    MPI_Barrier(MPI_COMM_WORLD);
	
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