#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// C code
//
enum STATE { OFF = 0, ON = 1, BROKEN = 2 };

struct Toaster {
  long length;
  long width;
  char* model;
  STATE state;
};

void initialize_toaster(Toaster& foo) {
  foo.length = 27;
  foo.width = 27;
  foo.model = (char*)"Top of the Line";
  foo.state = OFF;
}
void turn_on_toaster(Toaster& foo) {
  foo.state = ON;
}

// C++ code
//
class Super_Toaster {

  // public

  // protected 

  // private

};

int main (int argc, char** argv) {

  // C code
  //
  Toaster my_toaster;
  initialize_toaster(my_toaster);
  fprintf(stdout, "C: state is %d\n", my_toaster.state);
  turn_on_toaster(my_toaster);
  fprintf(stdout, "C: state is %d\n", my_toaster.state);

  // C++ code
  //
  Super_Toaster your_toaster;
}
