#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {

  char c;
  short int i;
  int j;
  long k;
  float x;
  double y;

  printf("sizeof c (char) = %ld bytes\n", sizeof(c));
  printf("sizeof i (short int) = %ld bytes\n", sizeof(i));
  printf("sizeof j (int) = %ld bytes\n", sizeof(j));
  printf("sizeof k (long int) = %ld bytes\n", sizeof(k));
  printf("sizeof x (float) = %ld bytes\n", sizeof(x));
  printf("sizeof y (double) = %ld bytes\n", sizeof(y));
  
}


