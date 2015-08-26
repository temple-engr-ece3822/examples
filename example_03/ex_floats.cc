#include <stdio.h>
#include <stdlib.h>

int main () {

  float value[3] = {-1, 0, 1};

  FILE* fp = fopen("x.dat", "w");
  fwrite(value, 4, 3, fp);
  fclose(fp);
}
