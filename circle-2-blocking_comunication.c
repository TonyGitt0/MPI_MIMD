#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc,&argv);
    int myrank, np;
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);


    int recived_rank;
    int sum = myrank;
    int num_received = myrank;
    
    if(myrank != np-1){
        MPI_Send(&myrank,1,MPI_INT,myrank+1,myrank+1,MPI_COMM_WORLD);
    }

    for(int i = 0; i < num_received; i++){
        MPI_Recv(&recived_rank,1,MPI_INT,myrank-1,myrank,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        sum += recived_rank;
        if(myrank != np-1){
            MPI_Send(&recived_rank,1,MPI_INT,myrank+1,myrank+1,MPI_COMM_WORLD);
        }
    }

    printf("Processo %d, somma: %d\n",myrank,sum);

    MPI_Finalize();
    return 0;


}