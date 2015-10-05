#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

enum STATE { OFF = 0, ON = 1, BROKEN = 2 };

// C++ code
//
class Super_Toaster {

  // protected 
  //
protected:
  long length;
  long width;
  char* model;
  STATE state;

  // public
  //
public:
  Super_Toaster(long length_a = 35, char* model_a = (char*)NULL);
  ~Super_Toaster();

  long set_length(long length_a);

  // private
  //
private:

};
