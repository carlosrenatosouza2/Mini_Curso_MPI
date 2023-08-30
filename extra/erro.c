#include <stdio.h>

// Exemplo de erro numerico simples

int main( int argc, char *argv[])
{
   int k, n;
   float x, s, se;

   //n = 100000;
   //x = 0.1;
   s = 0.0;
   n = 80000;
   x = 0.125;

   for (k = 0; k < n; k++)
      s += x;
   
   se = x * n;

   printf("n = %d\n", n);
   printf("x = %f\n", x);
   printf("Se = %f\n", se);
   printf("S  = %f\n", s);
   
   return 0;
}