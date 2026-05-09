#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tmeas.h"

#define DIM 5000
#define SIZE 25000000

void mat_vec(double* a, double* x, double* y, int n);

main ()
{
  static double a[SIZE],x[DIM],y[DIM],z[DIM];
  int n,i,j;
  const int ione=1;
  const double done=1.0;
  const double dzero=0.0;
  double t;

  for(i=0;i<SIZE;i++) a[i]=1.0*i;
  for(i=0;i<DIM;i++) x[i]=1.0*(DIM-i);

  n=DIM;

  printf("start\n");
  
  tstart();
  mat_vec(a,x,y,n);
  t=tstop();
  printf("end\n");
  printf("Time: %lf\n",t);

  // test
  printf("TEST\n");

  for(i=0;i<n;i++){
    z[i]=0.0;
    for(j=0;j<n;j++){
      z[i]+=a[i+n*j]*x[j];
    }
  }

  for(i=0;i<DIM;i++){
    if(fabs(y[i]-z[i])>1.e-9*z[i])
	 printf("Error %d!\n",i);
  }

}
