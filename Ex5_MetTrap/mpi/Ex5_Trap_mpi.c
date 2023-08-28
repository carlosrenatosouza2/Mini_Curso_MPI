#include<stdio.h>
#include <mpi.h>

double f(double x);
double trap(double local_a, double local_b, long int local_n, double h);

int main(int argc, char const *argv[])
{
    double a, b, h, integral, local_a, local_b, total;
    long int n, i, local_n;
    int r, p, tag = 1, source, dest = 0;
    
    MPI_Status status;
    
    a = 1.0;
    b = 2.0;
    n = 10000000000;
    h = (b - a) / n;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    local_n = n / p;                 // qtde de passos para cada rank
    local_a = r * local_n * h + a;   // inicio do intervalo de cada rank
    local_b = local_a + local_n * h; // fim do intervalo de cada rank
   
    // cada rank calcula a integral de sua parte:
    integral = trap(local_a, local_b, local_n, h);
        
    if ( r == 0 ) // rank 0 recebe o resultado de cada rank e soma tudo:
    { 
        total = integral;
        for ( source = 1; source < p; source ++)
        {
            MPI_Recv(&integral, 1, MPI_DOUBLE, source , tag, MPI_COMM_WORLD, &status);
            total = total + integral;
        }
    } 
    else // outros ranks enviam seu resultado para o rank 0:
    { 
        MPI_Send(&integral, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }

    // rank 0 imprime o resultado final:
    if ( r == 0 ) printf("\n Valor da integral definida eh: %6.4f\n ", total);

    MPI_Finalize();
    return 0;
}

double f(double x)
{
    double resultado;
    resultado = 1.0 / x;
    return(resultado);
}

double trap(double local_a, double local_b, long int local_n, double h)
{
    long int i;
    double integral, x;

    integral = (f(local_a) + f(local_b)) / 2.0;
    x = local_a;

    for (i = 1; i <= local_n-1; i++)
    {
        x = x + h;
        integral = integral + f(x);
    }
    
    integral = integral * h;

    return(integral);
}