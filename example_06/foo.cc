#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main (int argc, char** argv) {

  char* fname = argv[1];
  fprintf(stdout, "fname = [%s]\n", fname);

  long nvalues = atoi(argv[2]);
  bool is_stdin = false;

  // read from stdin
  //
  FILE* fp;

  if (strcmp(fname, "-") == 0) {
    is_stdin = true;
    fp = stdin;
  }
  else {
    is_stdin = false;
    fp = fopen(fname, "r");
  }

  //  
  //  if (fp == (FILE*)NULL) {
  //    fprintf(stdout, "error opening [%s]\n", fname);
  //    exit(-1);
  //  }

  short int new_buf[nvalues];
  long nvalues_read = 99;

  while (nvalues_read > 0) {
    nvalues_read = fread(new_buf, sizeof(short int), nvalues, fp);
    if (nvalues_read > 0) {
      fprintf(stdout, "num_values_read = %ld\n", nvalues_read);
      for (long i = 0; i < nvalues_read; i++) {
	fprintf(stdout, "%ld: value = [%d]\n", i, new_buf[i]);
      }
    }
  }

  if (is_stdin == false) {
    fclose(fp);
  }
}
