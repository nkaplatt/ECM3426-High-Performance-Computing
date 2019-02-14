#include <stdio.h>
#define axis 500
int main () {
  float tempValues[axis][axis];

  FILE *outfile;
  outfile=fopen("heat-transfer-init.dat","w");

  for (int i = 0; i < axis; i++) {
    for (int j = 0; j < axis; j++) {
      if (150 <= j && j <= 350 && 150 <= i && i <= 350) {
        tempValues[i][j] = 50.0;
      } else {
        tempValues[i][j] = 20.0;
      }
      fprintf(outfile,"%f %f %f\n", i * 0.0002, j * 0.0002, tempValues[i][j]);
    }
  }
  fclose(outfile);
  return 0;
}