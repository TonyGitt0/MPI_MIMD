#include <stdio.h>
#include <string.h> 
#include <mpi.h>

int main(int argc, char* argv[]){

    int rc;
    int rank;
    int count;
    int tag = 99;
    int mark[5] = {0,1,2,3,4}; 
    int numtasks;
    MPI_Status Stat;
    MPI_Init( &argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    

    if(rank == 0){
        //Una i è il rank master
        for(int i = 1; i < 4; i++) {
            MPI_Send(&mark, sizeof(mark)/sizeof(int), MPI_INT,i,i,MPI_COMM_WORLD);
        }
    }else{
        MPI_Recv(&mark, sizeof(mark)/sizeof(int), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG , MPI_COMM_WORLD, &Stat);
        int rc = MPI_Get_count(&Stat, MPI_INT, &count);
        printf("Process %d - recived %d int\n",rank,count);
    }
  
    MPI_Finalize();
    return 0;
}