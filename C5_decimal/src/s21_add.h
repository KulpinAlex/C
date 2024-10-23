#ifndef SRC_S21_ADD_H_
#define SRC_S21_ADD_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_decimal.h"

typedef struct big_decimal {
  unsigned bits[7];
} big_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int max_of_big_decimal(big_decimal value1, big_decimal value2);
int getSign(s21_decimal value);
int get_exponent(int value);
void set_exponent(unsigned int *value, int exp);
void SetBitFromUInt(unsigned int *number, int bit, int position);
big_decimal s21_decimal_to_big_decimal(s21_decimal value);
int s21_big_decimal_add(big_decimal value_1, big_decimal value_2,
                        big_decimal *result);
void s21_big_decimal_sub(big_decimal value_1, big_decimal value_2,
                         big_decimal *result);
big_decimal negtive_big_decimal(big_decimal value);
int s21_big_decimal_scale_ten(big_decimal *value);
int big_decimal_shift_left(big_decimal *value);
int big_decimal_shift_left_number(big_decimal *value, int position);
void big_decimal_shift_right(big_decimal *value);
void big_decimal_shift_right_number(big_decimal *value, int position);
void big_decimal_copy(big_decimal *value1, big_decimal value2);
int big_decimal_div_10(big_decimal *value);
void big_decimal_to_s21_decimal(big_decimal value1, s21_decimal *value2);
int normalize_big_decimal(big_decimal *value);

void big_decimal_shift_right_number(big_decimal *value, int position);
int s21_big_decimal_is_zero(big_decimal *dec);

#endif  // SRC_S21_ADD_H_