#include "foo.h"

Super_Toaster::Super_Toaster(long length_a, char* model_a) {
    length = length_a;
    model = new char[100];
    strcpy(model, model_a);
  }

Super_Toaster::~Super_Toaster() {
    delete [] model;
  }


long Super_Toaster::set_length(long length_a) {
    length = length_a;
    return length;
  }
