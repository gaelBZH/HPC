#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int number = 10;
const int write_to_file = 1; // true (1) or false (0)

main(){

  double a, b, c;
  int i,j,k,l;
  FILE *fptr;
  fptr = fopen("output.txt", "w");

  srand(time(NULL));

  printf("Insert two real numbers:\n");
  scanf("%lf %lf", &a, &b);
  printf("Random numbers from interval: %lf - %lf\n", a, b);

  for(i=0;i<number;i++)
  {
    c = (b-a)*(float)rand()/RAND_MAX + a;
    printf("%lf ",c);
    if (write_to_file)
    {
      fprintf(fptr, "%lf ", c);
    }

    #ifdef DEBUG
        if(c<a || c>b) {
          printf("Generation error %lf\n", c); exit(0);
        }
    #endif
  }
  if (write_to_file)
    {
      fprintf(fptr, "\n");
    }

  printf("\nInsert two integer numbers:\n");
  scanf("%d %d", &j, &k);
  printf("Random numbers from interval: %d - %d\n", j, k);

  for(i=0;i<number;i++)
  {
    l = rand() % (k-j) + j;
    printf("%d ",l);
    if (write_to_file)
    {
      fprintf(fptr, "%d ", l);
    }

    #ifdef DEBUG
        if(l<j || l>k) {
          printf("Generation error %d\n", l); exit(0);
        }
    #endif

  }
  if (write_to_file)
    {
      fprintf(fptr, "\n");
    }
  fclose(fptr); 

}
