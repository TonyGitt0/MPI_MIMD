#include <stdio.h>
#include <string.h> 
#include <mpi.h>

int main(int argc, char* argv[]){

    int myrank,numeroprocessi;
    MPI_Status Stat;
    MPI_Init( &argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numeroprocessi);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    

    if(myrank == 0){
        int mark[numeroprocessi-1];
        int count;
        int ricevuto = 0;
        for (int rank = 1 ; rank < numeroprocessi; rank++){
            MPI_Recv(&mark[rank-1],1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG , MPI_COMM_WORLD, &Stat);
            //in &count abbiamo l'elemento ricevuto dalla send
            MPI_Get_count(&Stat, MPI_INT, &count);
            ricevuto += count;
        }            

        printf("Numero di elementi ricevuti: %d\n", ricevuto);
        for(int i = 0; i< sizeof(mark)/sizeof(int); i++){
            printf("%d\n",mark[i]);
        }
    }
    else {
            srand(myrank * rand());
            int numero = rand() %20;
            MPI_Ssend(&numero,1,MPI_INT,0,myrank,MPI_COMM_WORLD);   
        }
    MPI_Finalize();
    return 0;
    
}