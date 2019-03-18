#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

const double a=0;
const double b=200;

int main(){
    int comm_sz;
    int my_rank;
    int local_n;
    double local_int, total_int;
    double n = 100;
    double h;
    double local_a;
    double local_b;


    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
    h=(b-a)/n;
    local_n = n/comm_sz;
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    double x;
    double rango = (local_a*local_a + local_b*local_b)/2.0;
    for(int i = 1; i < local_n; i++){
        x = local_a + i*h;
        rango += x*x;
    }
    rango = rango*h;
    

    if (my_rank == 0) {
        total_int = rango;
        for(int i = 1; i < comm_sz; i++){
            MPI_Recv(&local_int, 100, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int+=local_int;
        }
        printf("Area = %.f", total_int); 
    }else{
        total_int = rango;
        MPI_Send(&local_int, sizeof(local_int)+1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();

    return 0;
}
