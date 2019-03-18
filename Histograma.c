#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(void){
    int cont;
    int comm_sz;
    int my_rank;
    double array[] = {1.2,1.5,4.5,2.3,2.75,3.65,6.5,6.1,6.9,7.1,7.4,5.3,5.56,0.4,0.12,4.4};
    int n = sizeof(array)
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank!=0){
        cont = 0;
        for(int i = 0; i < sizeof(array); i++){
            if (array[i]>=my_rank && array[i] < my_rank+1) {
                cont++;
            }
        }
        MPI_Send(&cont, sizeof(cont)+1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }else{
        int cont2 = 0;
        for(int i = 0; i < n; i++){
            if (array[i]>=0 && array[i] < 1) {
                cont2++;
            }
        }

        printf("La frecuencia del rango %d - %d es: %d!\n", my_rank, my_rank+1, cont2);
        for(int i = 1; i < comm_sz; i++){
            MPI_Recv(&cont, 64, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("La frecuencia del rango %d - %d es: %d!\n", i, i+1, cont);
        }
    }
    
    MPI_Finalize();
    return 0;
}
