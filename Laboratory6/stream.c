// One file was easier than stream_d + second_wall  (One File to Rule them All)
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <sys/time.h>
#include <stdlib.h>

// Configuration of size N (compilation parameter)
#ifndef N
#define N 1000000
#endif

#define NTIMES 10
#define OFFSET 0

#define LINE "-------------------------------------------------------------\n"

#ifndef MIN
#define MIN(x,y) ((x)<(y)?(x):(y))
#endif
#ifndef MAX
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif


static double a[N+OFFSET];
static double b[N+OFFSET];
static double c[N+OFFSET];

static double rmstime[4] = {0};
static double maxtime[4] = {0};
static double mintime[4] = {FLT_MAX,FLT_MAX,FLT_MAX,FLT_MAX};

static char *label[4] = {"Copy:      ", "Scale:     ", "Add:       ", "Triad:     "};

// Time (second_wall.c)
double second()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)tp.tv_sec + (double)tp.tv_usec * 1e-6);
}

int checktick();









int main()
{
    int BytesSize;
    register int j;
    register int k;
    double scalar;
    double t;
    double times[4][NTIMES];

    // Calculate
    // Copy:  c=a       -> 1 read + 1 write = 2 * sizeof
    // Scale: b=s*c     -> 1 read + 1 write = 2 * sizeof
    // Add:   c=a+b     -> 2 read + 1 write = 3 * sizeof
    // Triad: a=b+s*c   -> 2 read + 1 write = 3 * sizeof
    double bytes[4] =
    {
        2 * (double)sizeof(double) * N,
        2 * (double)sizeof(double) * N,
        3 * (double)sizeof(double) * N,
        3 * (double)sizeof(double) * N
    };

    BytesSize = sizeof(double);
    printf(LINE);
    printf("STREAM\n");
    printf("Size of Arrays= %d\n", N);
    printf("Memory neeeded = %.1f MB.\n", (3.0 * N * BytesSize) / 1048576.0);
    printf(LINE);

    // Init
    for (j=0; j<N; j++)
    {
        a[j] = 1.0;
        b[j] = 2.0;
        c[j] = 0.0;
    }

    scalar = 3.0;
    for (k=0; k<NTIMES; k++)
    {
        // Copy
        times[0][k] = second();
        for (j=0; j<N; j++)
            c[j] = a[j];
        times[0][k] = second() - times[0][k];

        // Scale
        times[1][k] = second();
        for (j=0; j<N; j++)
            b[j] = scalar*c[j];
        times[1][k] = second() - times[1][k];

        // Add
        times[2][k] = second();
        for (j=0; j<N; j++)
            c[j] = a[j]+b[j];
        times[2][k] = second() - times[2][k];

        // Triad
        times[3][k] = second();
        for (j=0; j<N; j++)
            a[j] = b[j]+scalar*c[j];
        times[3][k] = second() - times[3][k];
    }

    // Results
    printf("Function      Rate (MB/s)   Min time     Max time\n");
    for (j=0; j<4; j++)
    {
        for (k=0; k<NTIMES; k++)
        {
            mintime[j] = MIN(mintime[j], times[j][k]);
            maxtime[j] = MAX(maxtime[j], times[j][k]);
        }
        printf("%s%11.4f  %11.6f  %11.6f\n", label[j], 1.0E-06 * bytes[j]/mintime[j], mintime[j], maxtime[j]);
    }
    return 0;
}


int checktick()
{
    int i;
    int minDelta;
    int Delta;
    double t1;
    double t2;
    double timesfound[20];
    for (i = 0; i < 20; i++)
    {
        t1 = second();
        while( ((t2=second()) - t1) < 1.0E-6 );
        timesfound[i] = t2;
    }
    minDelta = 1000000;
    for (i = 1; i < 20; i++)
    {
        Delta = (int)( 1.0E6 * (timesfound[i]-timesfound[i-1]));
        minDelta = MIN(minDelta, MAX(Delta,0));
    }
    return(minDelta);
}

// gcc -O3 -DN=10000000 stream.c -o stream_test -lm