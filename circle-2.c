#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc,&argv);
    int myrank, np;
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
   
    int num_received = myrank;
    //se è l'ultimo slave non invio nulla quindi  setto i numeri da inviare a zero
    //se invece ho altri slave il numero di elementi da inviare sarà uguale al myrank + 1
    // myrank = 0, 0+1 = 1 = numeri inviati 
    int num_sent = (myrank == np-1) ? 0 : myrank+1;
    MPI_Request received_requst[num_received], sent_request[num_sent];
    int received_renks[num_received];
    int sum = myrank;

    if(myrank != np-1){
        MPI_Isend(&myrank,1,MPI_INT, myrank+1, myrank+1, MPI_COMM_WORLD,&sent_request[myrank]);
    }

    for(int i = 0; i < num_received ; i++){
        MPI_Irecv(&received_renks[i],1,MPI_INT,myrank-1,myrank,MPI_COMM_WORLD,&received_requst[i]);
    }

    int complete;
    for(int i = 0; i < num_received; i++){
        //il processo N aspetta che la received sia completata
        MPI_Waitany(num_received, received_requst,&complete,MPI_STATUS_IGNORE);
        sum += received_renks[complete];
        if (myrank != np-1) {
            MPI_Isend(&received_renks[complete], 1, MPI_INT, myrank+1, myrank+1, MPI_COMM_WORLD, &sent_request[complete]);
        }
    }
    printf("Processo %d, somma: %d\n", myrank, sum);
 
    MPI_Finalize();

    return 0;
}