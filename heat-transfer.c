#include <stdio.h>
#define axis 500
#define timestep 0.0000009
#define alphasqrd 0.01

int main() {
  // 2D array
  float tempValues[axis][axis];

  // initial temperature landscape
  FILE *outfile;
  outfile=fopen("heat-transfer-init.dat","w");
  int i;
  int j;
  for (i = 0; i < axis; i++) {
    for (j = 0; j < axis; j++) {
      if (150 <= j && j <= 350 && 150 <= i && i <= 350) {
        tempValues[i][j] = 50.0;
      } else {
        tempValues[i][j] = 20.0;
      }
      fprintf(outfile,"%f %f %f\n", i * 0.0002, j * 0.0002, tempValues[i][j]);
    }
  }
  fclose(outfile);

  // formula (4)
  int x;
  for(x = 0; x < 10000; x++) {
    for (i = 1; i < axis; i++) {
      for (j = 1; j < axis; j++) {
        float rateOfChange = alphasqrd * (
          (tempValues[i+1][j] - 2 * tempValues[i][j] + tempValues[i-1][j]) / 0.0002 * 0.0002 + 
          (tempValues[i][j+1] - 2 * tempValues[i][j] + tempValues[i][j-1]) / 0.0002 * 0.0002
        );

        // formular (5)
        tempValues[i-1][j-1] += tempValues[i-1][j-1] + rateOfChange * timestep;
      }
    }
  }

  FILE *resultfile;
  resultfile=fopen("heat-transfer-result.dat","w");
  for (i = 0; i < axis; i++) {
    for (j = 0; j < axis; j++) {
      fprintf(resultfile,"%f %f %f\n", i * 0.0002, j * 0.0002, tempValues[i][j]);
    }
  }
  fclose(resultfile);

  return 0;
}