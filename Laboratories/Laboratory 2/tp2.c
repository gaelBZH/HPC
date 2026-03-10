#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int a = 10; // rows
const int b = 10; // cols

int randint(int start, int end)
{
  int l = rand() % (end-start) + start;
  return l;
}

double randfloat()
{
  double c = (double)rand();
  return c;
}

int main()
{
  // Number of numbers != 0 per row.
  int k = b / 5; // 20% of the line. i.e. 2 on 10 columns.
  if (k > b)
    err(1); // k can't be grater than the number of columns.

  // Creating the Matrix
  double **M;
  M = (double**)calloc(a, sizeof(double));
  for (int i = 0 ; i < a; i++)
    M[i] = (double*)calloc(b, sizeof(double));

  
  // Filling the Matrix
  for (int row = 0 ; row < a; row++)
  {
    // Generate k numbers on this line
    int generated_numbers = 0;
    while (generated_numbers < k)
    {
        int col = randint(0, b);
        if (M[row][col] == 0)
        {
          M[row][col] = (float)rand()/RAND_MAX;
          generated_numbers += 1;
        }
    } 
  }

  // Printing the Matrix
  for (int row = 0 ; row < a; row++)
  {
    for (int col = 0 ; col < b ; col++)
    {
      double value = M[row][col];
      if (value == 0.0)
        printf("   0    \t"); // Print '0' instead of '0.000000'
      else
        printf("%f\t", M[row][col]);
    }
    printf("\n");
  }

  // Record Created Matrix in CRS
  double* val = calloc(2*a, sizeof(double)); // 2a values != 0
  int* col_ind = calloc(2*a, sizeof(int));
  int* row_ptr = calloc(b, sizeof(int));

  int i = 0;
  for (int row = 0 ; row < a; row++)
  {
    for (int col = 0 ; col < b; col++)
    {
      double value = M[row][col];
      if (value == 0)
        continue;

      // If first value of the row
      if (row_ptr[row] == 0)
        row_ptr[row] = i;

      // Record the value and its column index into vectors
      
      val[i] = value;
      col_ind[i] = col;
      i += 1;
    }
  }

  // Print Vectors of the Recorded Matrix
  printf("\n\n\n");
  for (int i = 0; i < 2*a; i++)
    printf("%f  ", val[i]);
  printf("\n");
  for (int i = 0 ; i < 2*a; i++)
    printf("%i  ", col_ind[i]);
  printf("\n");
  for (int i = 0; i < b; i++)
    printf("%i  ", row_ptr[i]);
  printf("\n");

  

  // Free Memory
  for (int row = 0 ; row < a; row++)
    free(M[row]);
  free(M);


  return 0;
}

