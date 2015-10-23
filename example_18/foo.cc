#include "foo.h"

int main (int argc, char** argv) {

  // declare some variables
  //
  Car tcar;
  Car** cars = new Car*[10];
  char* tstr = new char[100];

  // call the function
  //
  strcpy(tstr, argv[1]);
  tcar.fun(cars, (char*)tstr);
  delete tstr;

  // display debug information
  //
  cars[1]->debug();
}

