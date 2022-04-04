#include <stdio.h>
#include <string.h> 
#include <mpi.h>

int main(int argc, char* argv[]){
    int N = 10;
    int np, myrank;
    int counter;
    
    MPI_Init( &argc, &argv);
    MPI_Status Stat;
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
   


    int division = (N / (np-1));
    int rest = (N%(np-1));
    int array[N];

  

    if(myrank == 0){
        int array_return[np-1];
        int max;
        int elements = 0;
            for(int i = 0; i < N; i++){
                array[i] = i;
                printf("%d\n",array[i]);
            }

                if (rest == 0){
                    int sub_array[division];
                    for(int rank = 1; rank < np; rank++){
                        memcpy(sub_array,&array[(rank-1) *division], division*sizeof(int));
                        MPI_Send(sub_array,division,MPI_INT,rank, rank, MPI_COMM_WORLD);
                    }
                }
                else{
                    int sub_array[division];
                    int sub_array_extra[division+rest];
                    for(int rank = 1; rank < np; rank++){
                        if(rank == (np-1)){
                            memcpy(sub_array_extra,&array[((rank-1) *division)], ((division+rest)*sizeof(int)));
                            MPI_Send(sub_array_extra, (division+rest) ,MPI_INT,rank, rank, MPI_COMM_WORLD);
                        }
                        else{
                            memcpy(sub_array,&array[((rank-1) *division)], ((division)*sizeof(int)));
                            MPI_Send(sub_array, division, MPI_INT, rank, rank, MPI_COMM_WORLD);
                        }
                    }
                }

            for(int rank = 1; rank< np; rank++){
                MPI_Recv(&array_return[rank-1],1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG , MPI_COMM_WORLD, &Stat);
                MPI_Get_count(&Stat,MPI_INT,&counter);
                elements = elements + counter;
            }
            
            //
            for(int i = 1; i < sizeof(array_return)/sizeof(int); i++){
                if(array_return[0]<array_return[i]){
                    array_return[0] = array_return[i];
                }
            }
            max = array_return[0];
            printf("MAX:%d",max);
           

    }
    else if(myrank == (np-1)){
            int sub_array_extra[division+rest];
            int sum;
            MPI_Recv(sub_array_extra, (division+rest), MPI_INT, 0 , myrank , MPI_COMM_WORLD, &Stat);
            MPI_Get_count(&Stat,MPI_INT,&counter);
                for(int i = 1; i< (division+rest); i++){
                    if(sub_array_extra[0]< sub_array_extra[i]){
                        sub_array_extra[0] = sub_array_extra[i];
                    }
                }
                int max = sub_array_extra[0];
                MPI_Send(&max,1,MPI_INT,0,myrank,MPI_COMM_WORLD);
        }
        else{
            int sub_array[division];
            int sum;
            MPI_Recv(sub_array, division, MPI_INT,0, myrank , MPI_COMM_WORLD, &Stat);
            MPI_Get_count(&Stat, MPI_INT, &counter);
                for(int i = 1; i< division; i++){
                    if(sub_array[0]< sub_array[i]){
                        sub_array[0]= sub_array[i];
                    }
                }
                int max = sub_array[0];
                MPI_Send(&max,1,MPI_INT,0,myrank,MPI_COMM_WORLD);
        }


        MPI_Finalize();
        return 0;
}