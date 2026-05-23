#include<stdio.h>
#include<omp.h>

#define N 500

int main()
{
   int square=0, number, i;
	
   printf("Insert number:\n");
   scanf("%d",&number);
   #pragma omp parallel for reduction(+:square)
   for(i = 0; i < N; i++)
   {
      square += number * number;
   }

   /*
   // Atomic (fast)
   #pragma omp parallel for
   for(i = 0; i < N; i++)
   {
      #pragma omp atomic
      square += number * number;
   }

   // Critical (slow)
   #pragma omp parallel for
   for(i = 0; i < N; i++)
   {
      #pragma omp critical
      {
         square += number * number;
      }
   }

   */
   
   printf("Square=%d\n",square);
}
