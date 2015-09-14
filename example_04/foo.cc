#include <stdio.h>
#include <stdlib.h>

bool calculate_0(float x) {
  printf(".. the value is %f\n", x);
  return true;
}

bool calculate_1(float x);


int main () {

  printf("... a driver program...\n");

  // call a function
  //
  bool status = calculate_0(27);

  // call a function
  //
  bool status_1 = calculate_1(27);
  
}

