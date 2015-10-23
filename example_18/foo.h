#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

class Car {

  // protected 
  //
protected:
  float speed;
  char* model;
  char* brand;

  // public
  //
public:
  Car(float arg = 60.0);
  ~Car();

  bool set_model(char* model_a);
  bool fun(Car** car, char* str);
  bool debug();

  // private
  //
private:

};
