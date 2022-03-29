#include <stdio.h>
#include <string.h> 
#include <mpi.h>

int main(int argc, char* argv[]){
    int N = 6;
    int numeroProcessi, myrank;
    int counter;
    
    MPI_Init( &argc, &argv);
    MPI_Status Stat;
    MPI_Comm_size(MPI_COMM_WORLD,&numeroProcessi);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    


    int divisione = (N / (numeroProcessi-1));
    int resto =(N%(numeroProcessi-1));
    int numeri[N];

  

    if(myrank == 0){
            for(int i = 0; i < N; i++){
                numeri[i] = i;
            }

                if (resto == 0){
                    int subArray[divisione];
                    for(int rank = 1; rank < numeroProcessi; rank++){
                        memcpy(subArray,&numeri[(rank-1) *divisione], divisione*sizeof(int));
                        MPI_Send(subArray,divisione,MPI_INT,rank, rank, MPI_COMM_WORLD);
                    }
                }
                else{
                    int subArray[divisione];
                    int subArrayExtra[divisione+resto];
                    printf("%d", resto);

                    for(int rank = 1; rank < numeroProcessi; rank++){
                        if(rank == (numeroProcessi-1)){
                            memcpy(subArrayExtra,&numeri[((rank-1) *divisione)], ((divisione+resto)*sizeof(int)));
                            MPI_Send(subArrayExtra, (divisione+resto) ,MPI_INT,rank, rank, MPI_COMM_WORLD);
                        }
                        else{
                            memcpy(subArray,&numeri[((rank-1) *divisione)], ((divisione)*sizeof(int)));
                            MPI_Send(subArray, divisione, MPI_INT, rank, rank, MPI_COMM_WORLD);
                        }
                    }
                }
    }
    else if(myrank == (numeroProcessi-1)){
            int subArrayExtra[divisione+resto];
            MPI_Recv(subArrayExtra, (divisione+resto), MPI_INT, 0 , myrank , MPI_COMM_WORLD, &Stat);
            int rc = MPI_Get_count(&Stat,MPI_INT,&counter);
            printf("Processo:%d ha ricevuto %d interi. ",myrank,counter);
            printf("stampiamo i numeri: ");
                for(int i = 0; i< (divisione+resto); i++){
                    printf("%d ",subArrayExtra[i]);
                }
                printf("\n");
        }
        else{
            int sub[divisione];
            MPI_Recv(sub, divisione, MPI_INT,0, myrank , MPI_COMM_WORLD, &Stat);
            int rc = MPI_Get_count(&Stat, MPI_INT, &counter);
            printf("Processo:%d ha ricevuto %d interi. ",myrank,counter);
            printf("stampiamo i numeri: ");
                for(int i = 0; i< divisione; i++){
                    printf("%d ",sub[i]);
                }
                printf("\n");
        }


        MPI_Finalize();
        return 0;
}