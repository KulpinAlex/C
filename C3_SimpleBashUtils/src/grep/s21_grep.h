#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define short_options "e:ivclnhsf:o"

typedef struct flags {
  int e;
  char option_e[1000];
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  char file_f[1000];
  int o;
  int err;
  char file[200][1000];
  int n_files;
} options;

void parsing_flags(int argc, char **argv, options *ptr);
void print_file(options *ptr);
void print_file_line(FILE *file, int line, char *n);
void print_file_o(char *buffer, char *file_name, options *ptr);
void buffer_opt(options *ptr, char *buffer, int size);

#endif  // SRC_GREP_S21_GREP_H_
