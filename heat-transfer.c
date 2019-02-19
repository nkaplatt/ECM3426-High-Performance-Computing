#include <stdio.h>
#define AXISINTERVAL 500
#define DELETATIME 0.0000009
#define ALPHASQRD 0.01
#define TIMESTEPS 10000
#define DELTAXY 0.0002

int main() {
  // 2D array
  double tempValues[AXISINTERVAL][AXISINTERVAL];

  // initial temperature landscape
  FILE *outfile;
  outfile=fopen("heat-transfer-init.dat","w");
  int i;
  int j;
  for (i = 0; i < AXISINTERVAL; i++) {
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

  FILE *testfile;
  double rateOfChange[AXISINTERVAL][AXISINTERVAL];
  int x;
  for(x = 0; x < TIMESTEPS; x++) {
    for (i = 1; i < AXISINTERVAL-1; i++) {
      for (j = 1; j < AXISINTERVAL-1; j++) {
        // formula (4)
        rateOfChange[i-1][j-1] = ALPHASQRD * (
          (tempValues[i+1][j] - 2 * tempValues[i][j] + tempValues[i-1][j]) / (DELTAXY * DELTAXY) + 
          (tempValues[i][j+1] - 2 * tempValues[i][j] + tempValues[i][j-1]) / (DELTAXY * DELTAXY)
        );
      }
    }

    for (i = 1; i < AXISINTERVAL; i++) {
      for (j = 1; j < AXISINTERVAL; j++) {
        // formular (5)
        tempValues[i][j] += rateOfChange[i-1][j-1] * DELETATIME;
      }
    }
  }

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