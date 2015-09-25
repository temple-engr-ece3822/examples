#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main (int argc, char** argv) {

  char* fname = argv[1];
  fprintf(stdout, "fname = [%s]\n", fname);

  FILE* fp = fopen(fname, "r");
  if (fp == (FILE*)NULL) {
    fprintf(stdout, "error opening [%s]\n", fname);
    exit(-1);
  }

  char buf[999];
  long nbytes = 50;
  long nbytes_read = fread(buf, sizeof(char), nbytes, fp);
  buf[nbytes] = (char)NULL;
  fprintf(stdout, "num_bytes_read = %ld\n", nbytes_read);
  fprintf(stdout, "value = [%s]\n", buf);

  fclose(fp);
}
