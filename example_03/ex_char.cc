#include <stdio.h>
#include <stdlib.h>

int main () {

  char* value = (char*)"JoePicone027";

  FILE* fp = fopen("x.dat", "w");
  fwrite(value, 1, 12, fp);
  fclose(fp);
}
