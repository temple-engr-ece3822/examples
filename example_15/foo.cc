#include <stdio.h>

bool jp_test(float x);
bool jp_test(float* x);
bool jp_test2(float* x);

int main (int argc, char** argv) {

  // call by value
  //
  fprintf(stdout, "call by value:\n");
  float x = 27;
  fprintf(stdout, " x before = %f\n", x);
  jp_test(x);
  fprintf(stdout, " x after = %f\n", x);
  fprintf(stdout, " ***> note the value in the calling program is not changed\n");

  // call by reference
  //
  fprintf(stdout, "call by reference:\n");
  x = 27;
  fprintf(stdout, " x before = %f\n", x);
  jp_test(&x);
  fprintf(stdout, " x after = %f\n", x);
  fprintf(stdout, " ***> note the value in the calling program IS changed\n");

  // call by reference
  //
  x = 27;
  fprintf(stdout, " x before = %f (memory: %lu)\n", x, (unsigned long)&x);
  jp_test2(&x);
  fprintf(stdout, " x after = %f (memory: %lu)\n", x, (unsigned long)&x);
  fprintf(stdout, " ***> note the value of the pointer is not changed\n");
}

bool jp_test(float arg) {
  arg = -1;
  return true;
}
bool jp_test(float* arg) {
  *arg = -1;
  return true;
}
bool jp_test2(float* arg) {
  arg = (float*)-1;
  // I could do *arg = 57, but that is dangerous because
  // we don't know what memory location -1 points to.
  // we will get a seg fault.
  return true;
}
