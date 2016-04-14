/***************************************************************************/
/* Asssignment 4 ***********************************************************/
/* Craig Carlson ***********************************************************/
/* Richard Pietrzak ********************************************************/
/* Greg Weil ***************************************************************/
/***************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<clcg4.h>
#include<mpi.h>
#include<pthread.h>
#include<assert.h>
#include<hwi/include/bqc/A2_inlines.h>

float** matrix = NULL;
float** transpose = NULL;
unsigned int rowsize = 0;
unsigned int chunk_size = 0;
unsigned int thread_count = 0;
unsigned int errcode = 0;
unsigned int ranks_per_file = 0;
unsigned int block_boundary = 0;
unsigned int BLOCK_SIZE = 8388608; //8MB block size
char* output_directory;

unsigned long long start_cycle_time=0;
unsigned long long end_cycle_time=0;
unsigned long long total_cycle_time=0;

void* thread_sum(void* input) {
	int* threadID = (int*)input;
	int ymin = (*threadID * rowsize) / thread_count;
	int ymax = ((*threadID + 1) * rowsize) / thread_count;
	for (int x = 0; x < chunk_size; ++x) {
		for (int y = ymin; y < ymax; ++y) {
			matrix[x][y] += transpose[x][y];
		}
	}
	return 0;
}

void output_single_file();
void output_multi_file();
void print_MPI_error();

int main(int argc, char *argv[]){
	MPI_Request request;
	MPI_Status status;
	int mpi_myrank;
	int mpi_commsize;	
	
	//initialize MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_commsize);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_myrank);
	InitDefault();//initialize RNG streams
	
	if(mpi_myrank == 0){
		//start compute timer
		start_cycle_time = GetTimeBase();
	}

	//ranks_per_file - self explanatory. To only output to 1 file, ranks_per_file should be equal to the number of ranks.
	//block_boundary - boolean, 0 means compact boundaries, 1 means 8 Mb block boundaries 
	rowsize = atoi(argv[1]);
	chunk_size = rowsize / mpi_commsize;
	thread_count = atoi(argv[2]);
	ranks_per_file = atoi(argv[3]);
	block_boundary = atoi(argv[4]); //bool, 1 or 0
	output_directory = argv[5];

	//initialize row data and send to other ranks
	matrix = calloc(chunk_size, sizeof(float*));
	transpose = calloc(chunk_size, sizeof(float*));
	//int testval = mpi_myrank*rowsize*chunk_size;
	for(int i=0; i<chunk_size; i++){
		//allocate row
		matrix[i] = calloc(rowsize, sizeof(float));
		transpose[i] = calloc(rowsize, sizeof(float));
		for(int j=0; j<rowsize; j++){
			matrix[i][j] = GenVal(i*mpi_myrank);
			//matrix[i][j] = testval++;
		}		
		//send row data to the rank that needs it
		for(int k=0; k<mpi_commsize; k++){
			MPI_Isend(&(matrix[i][k*chunk_size]), chunk_size, MPI_INT, k, mpi_myrank*i+i, MPI_COMM_WORLD, &request);
			MPI_Request_free(&request);
		}
	}
	
	//receive row data and construct transpose
	
	//temporary array for transpose construction
	float* tmp = calloc(chunk_size, sizeof(float));
	
	//from each rank, receive row_size/rank rows
	for(int j=0; j<mpi_commsize; j++){//for each rank
		for(int i=0; i<chunk_size; i++){//for each row
			MPI_Irecv(tmp, chunk_size, MPI_INT, j, j*i+i, MPI_COMM_WORLD, &request);
			MPI_Wait(&request, &status);
			for(int k=0;k<chunk_size;k++){
				//index of column in tmp == index of row in transpose
				transpose[k][j*chunk_size + i] = tmp[k];
			}
		}	
	}
	
	free(tmp);
	
	//finalize
	MPI_Barrier(MPI_COMM_WORLD);

	if(thread_count > 0){
		//do parallel addition
		pthread_t* threads = malloc(thread_count * sizeof(pthread_t));
		int* threadData = malloc(thread_count * sizeof(int));
		for (int i = 0; i < thread_count; ++i) {
			threadData[i] = i;
			pthread_create(&threads[i], NULL, thread_sum, &threadData[i]);
		}
		for (int i = 0; i < thread_count; ++i) {
			pthread_join(threads[i], NULL);
		}
		free(threads);
		free(threadData);
	}else{
		//do serial addition
		for (int x = 0; x < chunk_size; x++) {
			for (int y = 0; y < rowsize; y++) {
				matrix[x][y] += transpose[x][y];
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	
	if(mpi_myrank == 0){
		//end compute timer
		end_cycle_time = GetTimeBase();
		total_cycle_time = (end_cycle_time - start_cycle_time);
		double num_seconds = total_cycle_time / 1600000000.0; // convert to seconds with clock rate: 1.6ghz
		printf("Compute time - ranks: %d threads: %d time: %.10f\n",mpi_commsize, thread_count, num_seconds);
		
		//start I/O timer
		start_cycle_time = GetTimeBase();
	}

	if(mpi_commsize <= ranks_per_file){
		output_single_file(mpi_myrank);
	}
	else{
		output_multi_file(mpi_myrank);
	}
	
	if(mpi_myrank == 0){
		//end I/O timer
		end_cycle_time = GetTimeBase();
		total_cycle_time = (end_cycle_time - start_cycle_time);
		double num_seconds = total_cycle_time / 1600000000.0; // convert to seconds with clock rate: 1.6ghz
		printf("I/O time - ranks: %d threads: %d time: %.10f\n",mpi_commsize, thread_count, num_seconds);
	}
	
	//free all dynamically allocated memory
	for(int i=0; i<chunk_size; i++){
		free(matrix[i]);
		free(transpose[i]);
	}
	free(matrix);
	free(transpose);
	
    MPI_Finalize();
	
    return 0;
}

void print_MPI_error(int errcode, char *fun)
{       
	char errMessage[MPI_MAX_ERROR_STRING];
	int len;
	MPI_Error_string(errcode, errMessage, &len);
	printf("Error in %s: %s\n", fun, errMessage);
	MPI_Abort(MPI_COMM_WORLD, 1);
}

void output_single_file(int mpi_myrank){
	MPI_Status file_status;
	MPI_File fh;
	int offset_rank = 0;

	//TODO: add correct directory path to filename
	char filename[1024];
	sprintf(filename, "%s/output.txt",output_directory);

	errcode = MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
	if(errcode != MPI_SUCCESS){
		print_MPI_error(errcode, "MPI_File_open");
	}

	//output is binary, must format to read in terminal, replacing [rowlength] and [file]:
	//hexdump -v -e '[rowlength]/4 "%.2f "' -e '"\n"' [file]
	if(block_boundary){
		offset_rank = mpi_myrank*(rowsize*chunk_size*sizeof(float) + (BLOCK_SIZE - (rowsize*chunk_size*sizeof(float))%BLOCK_SIZE)%BLOCK_SIZE);
	}
	else{
		offset_rank = mpi_myrank*(rowsize*chunk_size*sizeof(float));
	}
	for (int row = 0; row < chunk_size; row++){
		int offset_row = row*(rowsize*sizeof(float));
		errcode = MPI_File_write_at_all(fh, offset_rank + offset_row, matrix[row], rowsize, MPI_FLOAT, &file_status);
		if(errcode != MPI_SUCCESS){
			print_MPI_error(errcode, "MPI_File_open");
		}
	}

	errcode = MPI_File_close(&fh);
	if(errcode != MPI_SUCCESS){
		print_MPI_error(errcode, "MPI_File_open");
	}

	MPI_Barrier(MPI_COMM_WORLD);
}

void output_multi_file(int mpi_myrank){
	MPI_Status file_status;
	MPI_File fh;
	int offset_rank = 0;

	MPI_Comm mpi_comm_file;
	int mpi_file_myrank;
	int mpi_file_commsize;

	int color = mpi_myrank / ranks_per_file;
	
	MPI_Comm_split(MPI_COMM_WORLD, color, mpi_myrank, &mpi_comm_file);

	MPI_Comm_rank(mpi_comm_file, &mpi_file_myrank);
	MPI_Comm_size(mpi_comm_file, &mpi_file_commsize);

	assert(mpi_file_commsize == ranks_per_file);

	//TODO: add correct directory path to filename
	char filename[1024];
	sprintf(filename, "%s/output_%04d.txt",output_directory,color);

	errcode = MPI_File_open(mpi_comm_file, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
	if(errcode != MPI_SUCCESS){
		print_MPI_error(errcode, "MPI_File_open");
	}

	//output is binary, must converted to read, replacing [rowlength] and [file]:
	//hexdump -v -e '[rowlength]/4 "%.2f "' -e '"\n"' [file]
	if(block_boundary){
		offset_rank = mpi_file_myrank*(rowsize*chunk_size*sizeof(float) + (BLOCK_SIZE - (rowsize*chunk_size*sizeof(float))%BLOCK_SIZE)%BLOCK_SIZE);
	}
	else{
		offset_rank = mpi_file_myrank*(rowsize*chunk_size*sizeof(float));
	}
	for (int row = 0; row < chunk_size; row++){
		int offset_row = row*(rowsize*sizeof(float));
		errcode = MPI_File_write_at(fh, offset_rank + offset_row, matrix[row], rowsize,	MPI_FLOAT, &file_status);
		if(errcode != MPI_SUCCESS){
			print_MPI_error(errcode, "MPI_File_open");
		}
	}

	errcode = MPI_File_close(&fh);
	if(errcode != MPI_SUCCESS){
		print_MPI_error(errcode, "MPI_File_open");
	}

	MPI_Barrier(MPI_COMM_WORLD);
}