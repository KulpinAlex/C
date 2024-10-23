#include "s21_grep.h"

int main(int argc, char **argv) {
  options *p = (options *)calloc(1, sizeof(options));
  parsing_flags(argc, argv, p);

  if (p->err != 1) {
    print_file(p);
  } else {
    printf("grep: invalid argument\n");
  }
  free(p);
  return 0;
}

// eivclnhsfo
void parsing_flags(int argc, char **argv, options *ptr) {
  int ch = 0;
  opterr = 0;
  while ((ch = getopt(argc, argv, short_options)) != -1) {
    if (ch == 'e') {
      ptr->e = 1;
      strcat(ptr->option_e, optarg);
    } else if (ch == 'i') {
      ptr->i = 1;
    } else if (ch == 'v') {
      ptr->v = 1;
    } else if (ch == 'c') {
      ptr->c = 1;
    } else if (ch == 'l') {
      ptr->l = 1;
    } else if (ch == 'n') {
      ptr->n = 1;
    } else if (ch == 'h') {
      ptr->h = 1;
    } else if (ch == 's') {
      ptr->s = 1;
    } else if (ch == 'f') {
      ptr->f = 1;
      strcat(ptr->file_f, optarg);
    } else if (ch == 'o') {
      ptr->o = 1;
    } else {
      ptr->err = 1;
    }
  }
  for (int i = 0; optind < argc; i++) {
    snprintf(ptr->file[i], sizeof(ptr->file[i]), "%s", argv[optind]);
    optind++;
    ptr->n_files++;
  }
}

void print_file_line(FILE *file, int line, char *n) {
  fseek(file, 0, SEEK_SET);
  int count = 1;
  char line_f[10001];
  line_f[0] = '\0';
  while (fgets(line_f, 10000, file) != NULL) {
    if (line == count) {
      printf("%s", line_f);
      if (strchr(line_f, '\n') == NULL) n[0] = 'd';
    }
    count++;
  }
}

void print_file_o(char *buffer, char *file_name, options *ptr) {
  regex_t reg;
  regmatch_t match;
  FILE *file_n = fopen(file_name, "r");
  if (file_n != NULL) {
    int reg_err;
    fseek(file_n, 0, SEEK_SET);
    if (ptr->i) {
      reg_err = regcomp(&reg, buffer, REG_EXTENDED | REG_ICASE);
    } else {
      reg_err = regcomp(&reg, buffer, REG_EXTENDED);
    }
    char line_n[10001];
    line_n[0] = '\0';
    int count = 1;
    while (fgets(line_n, 10000, file_n) != NULL) {
      strtok(line_n, "\n");
      int len_line = strlen(line_n);
      int start = 0;
      if (!ptr->e && !ptr->f) start = 1;
      if (reg_err == 0) {
        int r = 0;
        int success = 1;
        while ((success = regexec(&reg, line_n + r, 1, &match, 0)) == 0) {
          if (!ptr->h && ((ptr->n_files - start) != 1))
            printf("%s:", file_name);
          if (ptr->n) printf("%d:", count);
          for (int i = match.rm_so + r; i < match.rm_eo + r; i++) {
            if (line_n[i] != '\n') putchar(line_n[i]);
          }
          printf("\n");
          r += match.rm_eo;
          if (match.rm_so == match.rm_eo) {
            r += 1;
          }
          if (r > len_line) {
            break;
          }
        }
      }
      count++;
    }
    fclose(file_n);
  } else if (!ptr->s) {
    printf("grep: %s: No such file or directory", file_name);
  }
  regfree(&reg);
}

void print_file(options *ptr) {
  int cflags = 0;
  cflags = REG_EXTENDED;
  int size = 100000;
  char buffer[100000];
  buffer[0] = '\0';
  buffer_opt(ptr, buffer, size);
  regex_t reg;
  if (!ptr->err) {
    int reg_err;
    if (ptr->i) {
      reg_err = regcomp(&reg, buffer, REG_EXTENDED | REG_ICASE);
    } else {
      reg_err = regcomp(&reg, buffer, cflags);
    }
    int start = 0;
    if ((ptr->e == 0) && (ptr->f == 0)) start = 1;
    for (int i = start; i < ptr->n_files; i++) {
      char point_n[10];
      point_n[0] = '\0';
      int point_l = ptr->l;
      FILE *file_n = fopen(ptr->file[i], "r");
      if (file_n != NULL) {
        int result_lines[100000];
        int lines_number = 0;
        char line_n[10001];
        line_n[0] = '\0';
        int count_lines = 1;
        while (fgets(line_n, 10000, file_n) != NULL) {
          strtok(line_n, "\n");
          int success = 0;
          if (reg_err == 0) success = regexec(&reg, line_n, 0, NULL, 0);
          if (ptr->v) {
            if (success != 0) {
              result_lines[lines_number] = count_lines;
              lines_number++;
              if (point_l) {
                printf("%s\n", ptr->file[i]);
                point_l = 0;
              }
            }
          } else {
            if (success == 0) {
              result_lines[lines_number] = count_lines;
              lines_number++;
              if (point_l) {
                printf("%s\n", ptr->file[i]);
                point_l = 0;
              }
            }
          }
          count_lines++;
        }
        if (!ptr->l) {
          if (ptr->c) {
            if (ptr->h) {
              printf("%d\n", lines_number);
            } else if ((ptr->n_files - start) == 1) {
              printf("%d\n", lines_number);
            } else {
              printf("%s:%d\n", ptr->file[i], lines_number);
            }
          } else if (ptr->n) {
            if (!ptr->o) {
              for (int r = 0; r < lines_number; r++) {
                if (!ptr->h && ((ptr->n_files - start) != 1)) {
                  printf("%s:", ptr->file[i]);
                }
                printf("%d:", result_lines[r]);
                print_file_line(file_n, result_lines[r], point_n);
              }
              if (point_n[0] == 'd') {
                printf("\n");
              }
            } else {
              print_file_o(buffer, ptr->file[i], ptr);
            }
          } else if (ptr->h) {
            if (!ptr->o) {
              for (int r = 0; r < lines_number; r++) {
                print_file_line(file_n, result_lines[r], point_n);
              }
              if (point_n[0] == 'd') {
                printf("\n");
              }
            } else {
              print_file_o(buffer, ptr->file[i], ptr);
            }
          } else if (ptr->o) {
            if (!ptr->v) print_file_o(buffer, ptr->file[i], ptr);
          } else {
            for (int r = 0; r < lines_number; r++) {
              if ((ptr->n_files - start) != 1) {
                printf("%s:", ptr->file[i]);
              }
              print_file_line(file_n, result_lines[r], point_n);
            }
            if (point_n[0] == 'd') {
              printf("\n");
            }
          }
        }
        fclose(file_n);
      } else if (!ptr->s) {
        printf("grep: %s: No such file or directory", ptr->file[i]);
      }
    }
  }
  regfree(&reg);
}

void buffer_opt(options *ptr, char *buffer, int size) {
  if (ptr->f) {
    char line_f[10001];
    line_f[0] = '\0';
    FILE *file_f = fopen(ptr->file_f, "r");
    if (file_f != NULL) {
      char *t = "|";
      while (fgets(line_f, 10000, file_f) != NULL) {
        if ((int)(strlen(buffer) + strlen(line_f)) < size) {
          strtok(line_f, "\n");
          strcat(buffer, line_f);
          strcat(buffer, t);
        }
      }
      if (ptr->e) {
        strcat(buffer, ptr->option_e);
      } else {
        buffer[strlen(buffer) - 1] = '\0';
      }
      fclose(file_f);
    } else {
      printf("grep: %s: No such file or directory\n", ptr->file_f);
      ptr->err = 1;
    }
  } else if (ptr->e) {
    strcat(buffer, ptr->option_e);
  } else {
    strcat(buffer, ptr->file[0]);
  }
}
