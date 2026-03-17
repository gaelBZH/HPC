#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Compressed Row Storage

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
  srand(time(NULL));

  
  // Number of numbers different than 0 per row.
  int k = b / 5; // 20% of the line. i.e. 2 on 10 columns.
  if (k > b)
    return 1; // k can't be grater than the number of columns.


  // Creating the Matrix
  double **M;
  M = (double**)calloc(a, sizeof(double*));
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
  printf("--------- Original Matrix : ---------\n");
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
  double *val = calloc(a * k, sizeof(double));
  int *col_ind = calloc(a * k, sizeof(int));
  int *row_ptr = calloc(a + 1, sizeof(int));

  int i = 0;
  for (int row = 0; row < a; row++)
  {
      row_ptr[row] = i; 
      
      for (int col = 0; col < b; col++)
      {
          double value = M[row][col];
          if (value != 0)
          {
              val[i] = value;
              col_ind[i] = col;
              i++;
          }
      }
  }
  row_ptr[a] = i;

  
  // Print Vectors of the Recorded Matrix
  printf("\n\n--------- CRS ---------\nval[] = ");
  for (int i = 0; i < 2*a; i++)
    printf("%f  ", val[i]);
  printf("\n");
  printf("col_ind[] = ");
  for (int i = 0 ; i < 2*a; i++)
    printf("%i  ", col_ind[i]);
  printf("\n");
  printf("row_ptr[] = ");
  for (int i = 0; i < b; i++)
    printf("%i  ", row_ptr[i]);
  printf("\n\n\n");


  // Check the Equality
  printf("--------- Checking the Equality ---------\n");
  int match = 1; // boolean
  for (int r = 0; r < a; r++)
  {
      for (int c = 0; c < b; c++)
      {
          double dense_val = M[r][c];
          double crs_val = 0;

          // Search this column in the CRS
          for (int i = row_ptr[r]; i < row_ptr[r+1]; i++)
          {
              if (col_ind[i] == c)
              {
                  crs_val = val[i];
                  break;
              }
          }

          if (dense_val != crs_val)
          {
              match = 0;
              printf("Problem at (%i,%i). Original : %f, CRS : %f\n", r, c, dense_val, crs_val);
          }
      }
  }
  if (match)
    printf("Verification Successful, the two Matrix are equal.\n\n");  

  // Free Memory
  for (int row = 0 ; row < a; row++)
    free(M[row]);
  free(M);
  free(val);
  free(col_ind);
  free(row_ptr);
  printf("Memory has been freed.\n");

  return 0;
}

