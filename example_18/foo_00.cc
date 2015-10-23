#include "foo.h"

Car::Car(float speed_a) {
  speed = speed_a;
  model = (char*)NULL;
  brand = (char*)NULL;
}

Car::~Car() {
  delete [] model;
  delete [] brand;
}

bool Car::set_model(char* model_a) {
  model = model_a;
  return true;
}

bool Car::debug() {
  printf("hello world: %s\n", model);
  return true;
}

// implement a function
//
bool Car::fun(Car** car_a, char* str) {
  car_a[0] = new Car;
  car_a[0]->set_model(str);
  return true;
}
