#include "s21_comparison.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 1;
  big_decimal big_value_1 = s21_decimal_to_big_decimal(value_1);
  big_decimal big_value_2 = s21_decimal_to_big_decimal(value_2);
  int exp_1 = get_exponent(value_1.bits[3]);
  int exp_2 = get_exponent(value_2.bits[3]);
  int exp_sub = abs(exp_1 - exp_2);
  if (exp_sub) {
    if (exp_1 > exp_2) {
      for (int i = 1; i <= exp_sub; i++) {
        s21_big_decimal_scale_ten(&big_value_2);
      }
    } else {
      for (int i = 1; i <= exp_sub; i++) {
        s21_big_decimal_scale_ten(&big_value_1);
      }
    }
  }
  if (s21_decimal_is_zero(&value_1)) big_value_1.bits[6] = 0;
  if (s21_decimal_is_zero(&value_2)) big_value_2.bits[6] = 0;
  int sign_1 = GetBitFromUInt(big_value_1.bits[6], 31);
  int sign_2 = GetBitFromUInt(big_value_2.bits[6], 31);
  for (int i = 0; i < 6; i++) {
    if (big_value_1.bits[i] != big_value_2.bits[i]) result = 0;
  }
  if (result && (sign_1 != sign_2)) {
    result = 0;
  }

  return result;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !(s21_is_equal(value_1, value_2));
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int result = 1;
  big_decimal big_value_1 = s21_decimal_to_big_decimal(value_1);
  big_decimal big_value_2 = s21_decimal_to_big_decimal(value_2);
  int exp_1 = get_exponent(value_1.bits[3]);
  int exp_2 = get_exponent(value_2.bits[3]);
  int exp_sub = abs(exp_1 - exp_2);
  if (exp_sub) {
    if (exp_1 > exp_2) {
      for (int i = 1; i <= exp_sub; i++) {
        s21_big_decimal_scale_ten(&big_value_2);
      }
    } else {
      for (int i = 1; i <= exp_sub; i++) {
        s21_big_decimal_scale_ten(&big_value_1);
      }
    }
  }
  if (s21_decimal_is_zero(&value_1)) big_value_1.bits[6] = 0;
  if (s21_decimal_is_zero(&value_2)) big_value_2.bits[6] = 0;
  int sign_1 = GetBitFromUInt(big_value_1.bits[6], 31);
  int sign_2 = GetBitFromUInt(big_value_2.bits[6], 31);
  if (sign_1 && !sign_2) {
    result = 1;
  } else if (!sign_1 && sign_2) {
    result = 0;
  } else {
    int point = 0;
    for (int i = 5; i >= 0; i--) {
      if (big_value_1.bits[i] != big_value_2.bits[i]) {
        point = 1;
        if (big_value_1.bits[i] > big_value_2.bits[i]) {
          result = 0;
        } else {
          result = 1;
        }
        break;
      }
    }
    if (!point) result = 0;
    if (sign_1 == 1) result = !result;
  }
  if (s21_is_equal(value_1, value_2)) result = 0;

  return result;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_equal(value_1, value_2)) {
    result = 0;
  } else {
    result = !s21_is_less(value_1, value_2);
  }

  return result;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2)) {
    result = 1;
  }

  return result;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2)) {
    result = 1;
  }

  return result;
}