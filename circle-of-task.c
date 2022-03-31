#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[])
{   
    int soglia = 500;
    int myrank, np, counter;
    MPI_Status Stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    srand(myrank);
    int rounds[10];
    int tot = 0;
    int multiround;
    int report;

    for (int iteration = 0; iteration < 10; iteration++)
    {   
        int sum = 0;
        int round = 0;
        int prop = 0;
        while (1)
        {
            if (myrank != 0)
            {
                MPI_Recv(&sum, 1, MPI_INT, (myrank - 1), myrank - 1, MPI_COMM_WORLD, &Stat);
                if (sum < soglia)
                {
                    sum += rand() % 101;
                }
                else if (sum >= soglia)
                {
                    prop = sum;
                    MPI_Send(&prop, 1, MPI_INT, ((myrank + 1) % np), myrank, MPI_COMM_WORLD);
                }
                if (sum == prop)
                {
                    break;
                }
            }

            MPI_Send(&sum, 1, MPI_INT, ((myrank + 1) % np), myrank, MPI_COMM_WORLD);

            if (myrank == 0)
            {
                MPI_Recv(&sum, 1, MPI_INT, (np - 1), (np - 1), MPI_COMM_WORLD, &Stat);
                if (sum < soglia)
                {
                    round++;
                    sum += rand() % 101;
                }
                else if (sum >= soglia)
                {
                    //Mandiamo avanti tra i processi lo stesso numero 
                    prop = sum;
                    MPI_Send(&prop, 1, MPI_INT, ((myrank + 1) % np), myrank, MPI_COMM_WORLD);
                }
                if (sum == prop)
                {
                    //se un processo riceve un numero uguale alla cifra della somma attuale esce dal while
                    printf("iterazione: %d terminata in %d round.\n", iteration+ 1 , round);
                    rounds[iteration] = round;
                    if(iteration== 9){
                        for(int i = 0; i <10; i++){
                            multiround = rounds[i];
                            tot += multiround;
                        }
                        report = tot/10;
                        printf("Media round: %d\n",report);
                    }
                    break;
                }
            }
        }
    }
    MPI_Finalize();
    return 0;
}