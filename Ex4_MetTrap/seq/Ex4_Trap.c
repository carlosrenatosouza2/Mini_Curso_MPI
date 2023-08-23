#include<stdio.h>

float f(float x);

int main(int argc, char const *argv[])
{
    double a, b, h, x, integral;
    long int n, i;

    a = 1.0;
    b = 2.0;
    n = 10000000000;
    h = (b - a) / n;

    integral = (f(a) + f(b)) / 2.0;
    x = a;

    for (i = 1; i <= n-1; i++){
        x = x + h;
        integral = integral + f(x);
    }
    integral = integral * h;

    printf("\n Valor da integral definida eh: %6.4f\n ", integral);
    return 0;
}

float f(float x)
{
    float resultado;
    resultado = 1.0 / x;
    return(resultado);
}
