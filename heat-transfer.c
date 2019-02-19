#include <stdio.h>
#include <omp.h>

// #define AXISINTERVAL 500
// #define DELETATIME 0.0000009
// #define ALPHASQRD 0.01
// #define TIMESTEPS 10000
// #define DELTAXY 0.0002

int main() {
  // 2D array
  const int AXISINTERVAL = 500;
  const double DELETATIME = 0.0000009;
  const float ALPHASQRD = 0.01;
  const int TIMESTEPS = 10000;
  const float DELTAXY = 0.0002;
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
  FILE *outfile;
  int i;
  int j;
  omp_set_num_threads(2);
  #pragma omp parallel for default(none) shared(tempValues, outfile) private(i, j)
  for (i = 0; i < AXISINTERVAL; i++) {
    printf("thread num %d", omp_get_thread_num());
    for (j = 0; j < AXISINTERVAL; j++) {
      if (150 <= j && j <= 350 && 150 <= i && i <= 350) {
        tempValues[i][j] = 50.0;
      } else {
        tempValues[i][j] = 20.0;
      }
      fprintf(outfile,"%f %f %f\n", i * DELTAXY, j * DELTAXY, tempValues[i][j]);
    }
  }
  fclose(outfile);

  // FILE *testfile;
  // double rateOfChange[AXISINTERVAL][AXISINTERVAL];
  // int x;
  // for(x = 0; x < TIMESTEPS; x++) {
  //   for (i = 1; i < AXISINTERVAL-1; i++) {
  //     for (j = 1; j < AXISINTERVAL-1; j++) {
  //       // formula (4)
  //       rateOfChange[i-1][j-1] = ALPHASQRD * (
  //         (tempValues[i+1][j] - 2 * tempValues[i][j] + tempValues[i-1][j]) / (DELTAXY * DELTAXY) + 
  //         (tempValues[i][j+1] - 2 * tempValues[i][j] + tempValues[i][j-1]) / (DELTAXY * DELTAXY)
  //       );
  //     }
  //   }

  //   for (i = 1; i < AXISINTERVAL; i++) {
  //     for (j = 1; j < AXISINTERVAL; j++) {
  //       // formular (5)
  //       tempValues[i][j] += rateOfChange[i-1][j-1] * DELETATIME;
  //     }
  //   }
  // }

  // FILE *resultfile;
  // resultfile=fopen("heat-transfer-result.dat","w");
  // for (i = 0; i < AXISINTERVAL; i++) {
  //   for (j = 0; j < AXISINTERVAL; j++) {
  //     fprintf(resultfile,"%f %f %f\n", i * DELTAXY, j * DELTAXY, tempValues[i][j]);
  //   }
  // }
  // fclose(resultfile);

  return 0;
}