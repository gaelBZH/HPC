#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int a = 10; // rows
const int b = 10; // cols
const int n = 2; // Number of numbers != 0 per row.

int randint(int start, int end)
{
  int l = rand() % (end-start) + start;
  return l;
}

float randfloat()
{
  float c = (float)rand();
  return c;
}

int main()
{
  // TODO : INPUT, n can't be greater than b

  // Creating the Matrix
  int **M;
  M = (int**)calloc(a, sizeof(int));
  for (int i = 0 ; i < a; i++)
    M[i] = (int*)calloc(b, sizeof(int));

  
  // Filling the Matrix
  for (int row = 0 ; row < a; row++)
  {
    // Generate n numbers on this line
    int generated_numbers = 0;
    while (generated_numbers < n)
    {
        int col = randint(0, b);
        if (M[row][col] == 0)
        {
          M[row][col] = randfloat();
          generated_numbers += 1;
        }
    } 
  }

  // Printing the Matrix
  for (int row = 0 ; row < a; row++)
  {
    for (int col = 0 ; col < b ; col++)
    {
      printf("%i\t", M[row][col]);
    }
    printf("\n");
  }


  return 0;
}
