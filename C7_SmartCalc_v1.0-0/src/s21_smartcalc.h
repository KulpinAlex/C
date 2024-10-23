#ifndef S21_SMARTCALC_H
#define S21_SMARTCALC_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S21_PI 3.141592653589793238462643383279502884L

typedef enum {
  type_num,
  type_x,
  type_bkt_open,
  type_bkt_close,
  type_sum,
  type_sub,
  type_mul,
  type_div,
  type_pow,
  type_mod,
  type_cos,
  type_sin,
  type_tan,
  type_acos,
  type_asin,
  type_atan,
  type_sqrt,
  type_ln,
  type_log
} type_t;

typedef enum {
  p_num_or_x,
  p_sum_sub,
  p_mul_div_mod,
  p_pow,
  p_func,
  p_bkt
} priority;

typedef struct {
  double value;
  type_t type;
  priority pr;
} Data;

typedef struct list {
  Data Data;
  struct list *next;
} list;

typedef struct lin_list {
  struct list *first;
  struct list *last;
} lin_list;

void s21_clear_space(char *str, char *sdt);
int s21_check_str(char *str);
int s21_check_num(char *str);
int s21_check_ch(char *str, const char *dst, int size);
int s21_check_ch_a(char *str);
int s21_check_ch_cmt(char *str, char ch);
int s21_check_bkt_oc(char *str, char ch, char last);
int s21_check_func_inp(char *str, char ch);
int s21_check_ch_end(char *str, char ch);
void s21_check_unary(char *str, char *sdt);
int s21_parser(char *str, lin_list **output);
int s21_pop_list(list **head, Data *data);
int s21_pop_lin_list(lin_list **head, Data *data);
int s21_push_list(list **head, Data data);
int s21_push_lin_list(lin_list **head, Data data);
void s21_free_lin_list(lin_list **head);
void s21_free_list(list **head);
int s21_parser_switch(lin_list **head, char ch, list **stack, char **temp);
int s21_check_switch(lin_list **head, list **stack, Data *data);
void s21_data_ch_s(Data *data, char **temp, int *shift);
void s21_data_ch_a(Data *data, char **temp, int *shift);
void s21_data_ch_l(Data *data, char **temp, int *shift);
void s21_print_lin_list(lin_list **output);
int s21_calculate(lin_list **output, double x, double *answer);
int s21_calc_binary(Data *val1, Data *val2, int type);
int s21_calc_function(Data *val1, int type);
int s21_smartcalc(char *str, double x, double *answer);

#endif  // S21_SMARTCALC_H