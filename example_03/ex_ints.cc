#include <stdio.h>
#include <stdlib.h>

int main () {

  short int value[10] = {-2, -1, 0, 1, 2, 3};

  FILE* fp = fopen("x.dat", "w");
  fwrite(value, 2, 6, fp);
  fclose(fp);
}
