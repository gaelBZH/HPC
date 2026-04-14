#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <err.h>

// Method : Compressed Row Storage (CRS)

const int a = 10; // rows
const int b = a; // cols

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

  // Creating the Vector and Filling it
  double *T = calloc(a, sizeof(double));
  for (int i = 0; i < a; i++)
    T[i] = (float)rand()/RAND_MAX;


  // Printing maximum : 15x15
  if (a <= 15 && b <= 15)
  {
    // Printing the Matrix
    printf("--------- Original Matrix (%ix%i) : ---------\n", a, b);
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

    // Printing the Vector
    printf("\n\n\n--------- Vector (%i) : ---------\n", a);
    for (int i = 0; i < a; i++)
      printf("%f\t", T[i]);
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
  printf("\n\n--------- CRS Matrix ---------\nval[] = ");
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
  printf("--------- Checking the Equality beetween the two Matrix ---------\n");
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
    printf("Verification Successful, the two Matrix are equal.\n\n\n");  


  // for both standard and CRS multiplications,
  // results are initialized with 0 values (calloc)
  // so we don't need local "sum" variables, we only do
  // vector += partial sum
  
  // Standard Multiplication
  printf("--------- Standard Multiplication M x T ---------\n");
  double* standard_result = calloc(a, sizeof(double));
  for (int i = 0; i < a; i++)
  {
    for (int j = 0; j < b; j++)
      standard_result[i] += M[i][j] * T[j];
    printf("%f\t", standard_result[i]);
  }
  printf("\n\n\n");

  // CRS Multiplication
  printf("--------- CRS Multiplication M x T ---------\n");
  double* crs_result = calloc(a, sizeof(double));
  for (int i = 0; i < a; i++)
  {
    for (int k = row_ptr[i]; k < row_ptr[i+1]; k++)
      crs_result[i] += val[k] * T[col_ind[k]];
    printf("%f\t", standard_result[i]);
  }
  printf("\n\n\n");

  // Procedure for Verification of the Results
  printf("--------- Procedure for Verification of the Results ---------\n");
  for (int i = 0; i < a; i++)
    if (standard_result[i] != crs_result[i])
      errx(EXIT_FAILURE, "Verification failed. Error on i=%i. Got standard_result[%i]=%f and crs_result[%i]=%f", i, i, standard_result[i], i, crs_result[i]);
  printf("Verification Successfull, the two results of matrix-vector products are equal.");
  printf("\n\n\n");

  // Free Memory
  printf("--------- Freeing Memory ---------\n");
  for (int row = 0 ; row < a; row++)
    free(M[row]);
  free(M);
  free(val);
  free(col_ind);
  free(row_ptr);
  free(T);
  free(standard_result);
  free(crs_result);
  printf("Memory has been freed.\n");

  return 0;
}

