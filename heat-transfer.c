#include <stdio.h>
#include <omp.h>
#include <time.h>

int main() {
  // 2D array
  int AXISINTERVAL = 500;
  double DELETATIME = 0.0000009;
  float ALPHASQRD = 0.01;
  int TIMESTEPS = 10000;
  float DELTAXY = 0.0002;
  double tempValues[AXISINTERVAL][AXISINTERVAL];

  omp_set_num_threads(2);
  int sum = 0;
  int b = 0;
  #pragma omp parallel for default(none) shared(sum) private(b)
  for (b = 0; b < 50; b++) {
    sum += b;
  }
  printf("%i", sum);

  // initial temperature landscape
  int i;
  int j;
  #ifdef _OPENMP
    omp_set_num_threads(4);
    double start_t = omp_get_wtime();
  #else
    double start_t = clock();
  #endif

  #pragma omp parallel for private(i, j)
  for (i = 0; i < AXISINTERVAL; i++) {
    printf("thread num %d", omp_get_thread_num());
    for (j = 0; j < AXISINTERVAL; j++) {
      if (150 <= j && j <= 350 && 150 <= i && i <= 350) {
        tempValues[i][j] = 50.0;
      } else {
        tempValues[i][j] = 20.0;
      }
    }
  }

  #ifdef _OPENMP
    printf("Time spent: %f\n", omp_get_wtime() - start_t);
  #else
    printf("Time spent: %f\n", clock() - start_t);
  #endif

  FILE *outfile;
  outfile=fopen("heat-transfer-init.dat","w");
  for (i = 0; i < AXISINTERVAL; i++) {
    for (j = 0; j < AXISINTERVAL; j++) {
      fprintf(outfile,"%f %f %f\n", i * DELTAXY, j * DELTAXY, tempValues[i][j]);
    }
  }
  fclose(outfile);

  FILE *testfile;
  double rateOfChange[AXISINTERVAL][AXISINTERVAL];
  int x;
  #ifdef _OPENMP
    omp_set_num_threads(4);
    start_t = omp_get_wtime();
  #else
    start_t = clock();
  #endif

  for(x = 0; x < TIMESTEPS; x++) {
    #pragma omp parallel for private(i, j)
    for (i = 1; i < AXISINTERVAL-1; i++) {
      for (j = 1; j < AXISINTERVAL-1; j++) {
        // formula (4)
        rateOfChange[i-1][j-1] = ALPHASQRD * (
          (tempValues[i+1][j] - 2 * tempValues[i][j] + tempValues[i-1][j]) / (DELTAXY * DELTAXY) + 
          (tempValues[i][j+1] - 2 * tempValues[i][j] + tempValues[i][j-1]) / (DELTAXY * DELTAXY)
        );
      }
    }
    #pragma omp parallel for private(i, j)
    for (i = 1; i < AXISINTERVAL; i++) {
      for (j = 1; j < AXISINTERVAL; j++) {
        // formular (5)
        tempValues[i][j] += rateOfChange[i-1][j-1] * DELETATIME;
      }
    }
  }

  #ifdef _OPENMP
    printf("Time spent: %f\n", omp_get_wtime() - start_t);
  #else
    printf("Time spent: %f\n", clock() - start_t);
  #endif

  FILE *resultfile;
  resultfile=fopen("heat-transfer-result.dat","w");
  for (i = 0; i < AXISINTERVAL; i++) {
    for (j = 0; j < AXISINTERVAL; j++) {
      fprintf(resultfile,"%f %f %f\n", i * DELTAXY, j * DELTAXY, tempValues[i][j]);
    }
  }
  fclose(resultfile);

  return 0;
}