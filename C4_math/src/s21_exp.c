#include "s21_math.h"

long double s21_exp(double x) {
  long double result = 1.;
  return (check_exp(x, &result)) ? process_exp(x) : result;
}

long double process_exp(double x) {
  long double result = 1.0;
  int sign = (x < 0) ? -1 : 1;
  x = s21_fabs(x);
  long double add_value = 1.0;
  long double count = 1;
  while (s21_fabs(add_value) > 1e-15) {
    add_value *= x / count;
    result += add_value;
    if (result > S21_DBL_MAX) {
      result = S21_INF;
      break;
    }
    count++;
  }

  if (sign == -1) {
    result = 1.0 / result;
  }

  return result;
}

int check_exp(double x, long double *result) {
  int ok = 1;
  if (s21_isnan(x) || x == S21_INF) {
    *result = x;
    ok = 0;
  } else if (x == S21_NINF) {
    *result = 0.0;
    ok = 0;
  }
  return ok;
}