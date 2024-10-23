#ifndef SRC_S21_MUL_H_
#define SRC_S21_MUL_H_

#include "s21_add.h"
#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

void s21_big_decimal_mul(s21_decimal value_1, s21_decimal value_2,
                         big_decimal *result);
void s21_big_decimal_div(big_decimal value_1, s21_decimal value_2,
                         big_decimal *result);
int s21_div_body_int(s21_decimal *dec, int a);
void s21_maximize_big_dec(big_decimal *dec);
void s21_optimize_dec(s21_decimal *dec);
int is_less_body(big_decimal big_value_1, big_decimal big_value_2);
int is_possible_to_div(big_decimal a, s21_decimal b);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod_help(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

#endif  // SRC_S21_MUL_H_