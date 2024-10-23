#include "s21_cat.h"

int main(int argc, char **argv) {
  options *p = (options *)calloc(1, sizeof(options));
  parsing_flags(argc, argv, p);

  if (p->err != 1) {
    int point = 0;
    int count = 1;
    int ch_end = 10;
    while (point < p->n_files && p->n_files != 0) {
      FILE *file = fopen(p->file[point], "r");
      if (file == NULL) {
        printf("%s: No such file or directory\n", p->file[point]);
      } else {
        print_file(file, p, &count, &ch_end);
        fclose(file);
      }
      point++;
    }
  } else {
    printf("invalid option\n");
  }

  free(p);
  return 0;
}

void parsing_flags(int argc, char **argv, options *ptr) {
  int ch = 0;
  opterr = 0;
#ifdef __APPLE__
  while ((ch = getopt(argc, argv, short_options)) != -1) {
    if (ch == 'b')
      ptr->b = 1;
    else if (ch == 'e')
      ptr->e = 1;
    else if (ch == 'n')
      ptr->n = 1;
    else if (ch == 's')
      ptr->s = 1;
    else if (ch == 't')
      ptr->t = 1;
    else if (ch == 'v')
      ptr->v = 1;
    else
      ptr->err = 1;
  }
#else
  while ((ch = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {
    if (ch == 'b')
      ptr->b = 1;
    else if (ch == 'e')
      ptr->e = 1;
    else if (ch == 'E')
      ptr->E = 1;
    else if (ch == 'n')
      ptr->n = 1;
    else if (ch == 's')
      ptr->s = 1;
    else if (ch == 't')
      ptr->t = 1;
    else if (ch == 'T')
      ptr->T = 1;
    else if (ch == 'v')
      ptr->v = 1;
    else
      ptr->err = 1;
  }
#endif
  for (int i = 0; optind < argc; i++) {
    snprintf(ptr->file[i], sizeof(ptr->file[i]), "%s", argv[optind]);
    optind++;
    ptr->n_files++;
  }
}

void print_file(FILE *file, options *ptr, int *count, int *ch_end) {
  ptr->n = (ptr->b) ? 0 : ptr->n;
  ptr->v = (ptr->e) ? 1 : ptr->v;
  ptr->v = (ptr->t) ? 1 : ptr->v;
  int ch;
  int point = 0;
  int point_s = 0;
  while ((ch = fgetc(file)) != EOF) {
    if (ptr->s) {
      if (ch == '\n') {
        point_s++;
      } else {
        point_s = 0;
      }
    }
    if (ptr->n) {
      if (*ch_end != '\n') {
        point = 1;
      }
      if (point == 0 && point_s < 3) {
        printf("%6d\t", *count);
        point = (ch == '\n') ? 0 : 1;
        (*count)++;
      } else if (ch == '\n') {
        point = 0;
      }
    }
    if (ptr->b) {
      if (*ch_end != '\n') {
        point = 1;
      }
      if (point == 0 && ch != '\n') {
        printf("%6d\t", *count);
        point = 1;
        (*count)++;
      } else if (ch == '\n') {
        point = 0;
      }
    }

    if (ptr->e || ptr->E) {
      if (ch == '\n' && point_s < 3) {
        printf("$");
      }
    }

    if (ptr->v) {
      if (ch >= 0 && ch < 32 && ch != '\n' && ch != '\t') {
        printf("^");
        ch += 64;
      }
      if (ch == 127) {
        ch -= 64;
        printf("^");
      }
      if (ch > 127 && ch < 160) {
        ch -= 64;
        printf("M-^");
      }
      if (ch >= 160 && ch < 255) {
        ch -= 128;
        printf("M-");
      }
      if (ch == 255) {
        ch -= 192;
        printf("M-^");
      }
    }
    if (point_s < 3) {
      if ((ptr->t || ptr->T) && ch == '\t')
        printf("^I");
      else
        printf("%c", ch);
    }
    *ch_end = ch;
  }
}
