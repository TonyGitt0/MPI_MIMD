#include <stdio.h>
#include <string.h> 
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>


#define NUM_ELEMENTS 100000000
                     
int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    int myrank, np;

    MPI_Comm_size(MPI_COMM_WORLD,&np);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

    double total_time_gather = 0.0;
    float* received_data = (float*)malloc(sizeof(float) * NUM_ELEMENTS);
    float* send_data = (float*)malloc(sizeof(float) * NUM_ELEMENTS/np);

    MPI_Barrier(MPI_COMM_WORLD);
    total_time_gather -= MPI_Wtime();
    MPI_Gather(send_data , NUM_ELEMENTS/np , MPI_FLOAT, received_data, NUM_ELEMENTS/np , MPI_FLOAT , 0 , MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    total_time_gather += MPI_Wtime();

    free(received_data);
    free(send_data);

    if(myrank == 0){
        printf("Time gather: %f", total_time_gather);
    }
 

    MPI_Finalize();
    return 0;
}