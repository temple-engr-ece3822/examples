#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

enum DEBUG { NONE = 0, BRIEF, DETAILED, ALL, DEF_DEBUG = NONE };

struct Signal {
  long nsamples;
  DEBUG dbg;
  float values[999];
};

int main (int argc, char** argv) {

  DEBUG dbg = DEF_DEBUG;

  if (strcmp(argv[1], "brief") == 0) {
    dbg = BRIEF;
  }
  else if (strcmp(argv[1], "detailed") == 0) {
    dbg = DETAILED;
  }
  else if (strcmp(argv[1], "all") == 0) {
    dbg = ALL;
  }
  else {
    fprintf(stdout, "foo: argument value unknown [%s]\n", argv[1]);
  }
  printf("dbg = %lu\n", (unsigned long)dbg);
  
  //  dbg = MICHAEL;
  Signal my_sig;
  my_sig.nsamples = 27;
  my_sig.dbg = ALL;
  my_sig.values[0] = 35.0;
}
