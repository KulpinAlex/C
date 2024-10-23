#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define short_options "beEnstTv"

typedef struct flags {
  int b;
  int e;
  int E;
  int n;
  int s;
  int t;
  int T;
  int v;
  int err;
  char file[200][1000];
  int n_files;
} options;

const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {NULL, 0, NULL, 0}};

void parsing_flags(int argc, char **argv, options *ptr);
void print_file(FILE *file, options *ptr, int *count, int *ch_end);

#endif  // SRC_CAT_S21_CAT_H_
