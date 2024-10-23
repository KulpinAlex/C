#include "s21_another_functions.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
  SetBitFromUInt(&result->bits[3], !GetBitFromUInt(value.bits[3], 31), 31);
  if ((value.bits[0] == 0) && (value.bits[1] == 0) && (value.bits[2] == 0)) {
    result->bits[3] = 0;
  }
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  big_decimal big_result = s21_decimal_to_big_decimal(value);
  int exp = get_exponent(big_result.bits[6]);
  if (exp > 0) {
    for (int i = 0; exp > 0; i++, exp--) {
      big_decimal_div_10(&big_result);
    }
    big_decimal_to_s21_decimal(big_result, result);
  } else {
    for (int i = 0; i < 4; i++) {
      result->bits[i] = value.bits[i];
    }
  }
  if ((result->bits[0] == 0) && (result->bits[1] == 0) &&
      (result->bits[2] == 0)) {
    result->bits[3] = 0;
  }
  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  big_decimal big_result = s21_decimal_to_big_decimal(value);
  s21_decimal decimal_one = {{1, 0, 0, 0}, 0};
  SetBitFromUInt(&decimal_one.bits[3], 1, 31);
  int exp = get_exponent(big_result.bits[6]);
  int sign = GetBitFromUInt(value.bits[3], 31);
  int point = 0;
  if (exp > 0) {
    for (int i = 0; exp > 0; i++, exp--) {
      if (point == 0) {
        point = big_decimal_div_10(&big_result);
      } else {
        big_decimal_div_10(&big_result);
      }
    }
    big_decimal_to_s21_decimal(big_result, result);
  } else {
    for (int i = 0; i < 4; i++) {
      result->bits[i] = value.bits[i];
    }
  }
  if ((point != 0) && sign) {
    s21_add(*result, decimal_one, result);
  }
  if ((result->bits[0] == 0) && (result->bits[1] == 0) &&
      (result->bits[2] == 0)) {
    result->bits[3] = 0;
  }
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  big_decimal big_result = s21_decimal_to_big_decimal(value);
  s21_decimal decimal_one = {{1, 0, 0, 0}, 0};
  int exp = get_exponent(big_result.bits[6]);
  int sign = GetBitFromUInt(value.bits[3], 31);
  SetBitFromUInt(&decimal_one.bits[3], sign, 31);
  int point = 0;
  if (exp > 0) {
    for (int i = 0; exp > 0; i++, exp--) {
      point = big_decimal_div_10(&big_result);
    }
    big_decimal_to_s21_decimal(big_result, result);
  } else {
    for (int i = 0; i < 4; i++) {
      result->bits[i] = value.bits[i];
    }
  }
  if (point > 4) {
    s21_add(*result, decimal_one, result);
  }
  if ((result->bits[0] == 0) && (result->bits[1] == 0) &&
      (result->bits[2] == 0)) {
    result->bits[3] = 0;
  }
  return 0;
}