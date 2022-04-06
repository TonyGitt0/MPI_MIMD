#include <stdio.h>
#include <string.h> 
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>


#define NUM_ELEMENTS 100000000
                     
int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    int np,myrank;
    double total_time_my_bcast = 0.0;
    double total_time_bcast = 0.0;
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    
    int num_elements = 50; float* data = (float*)malloc(sizeof(float) * NUM_ELEMENTS);
    assert(data != NULL);

    MPI_Barrier(MPI_COMM_WORLD);
    total_time_bcast -= MPI_Wtime();
    MPI_Bcast(data,NUM_ELEMENTS,MPI_FLOAT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    total_time_bcast += MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    total_time_my_bcast -= MPI_Wtime();
    my_bcast(data,NUM_ELEMENTS,MPI_FLOAT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    total_time_my_bcast += MPI_Wtime();



    if(myrank == 0){
        printf("Data size: %d\n", NUM_ELEMENTS * (int)sizeof(int));
        printf("Time general_bcast: %f\n",total_time_bcast);
        printf("Time my_bcast: %f\n",total_time_my_bcast);
    }
    free(data);
    MPI_Finalize();
    return 0;
}

void my_bcast (void* data, int count, MPI_Datatype datatype,int root,MPI_Comm comunicator){
    int world_rank, world_size;
    MPI_Comm_size(comunicator,&world_size);
    MPI_Comm_rank(comunicator,&world_rank);

    if(world_rank == root){
        int i;
        for(i=0; i<world_size; i++){
            if( i != world_rank){
                MPI_Send(data,count,datatype,i,0,comunicator);
            }
        }
    }else{
        MPI_Recv(data,count,datatype,root,0,comunicator,MPI_STATUS_IGNORE);
    }
}

