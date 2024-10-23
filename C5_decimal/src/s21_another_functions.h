#ifndef SRC_S21_ANOTHER_FUNCTIONS_H_
#define SRC_S21_ANOTHER_FUNCTIONS_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_add.h"
#include "s21_comparison.h"
#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif  // SRC_S21_ANOTHER_FUNCTIONS_H_