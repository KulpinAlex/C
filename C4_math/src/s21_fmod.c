#include "s21_math.h"

long double s21_fmod(double x, double y) {
  if (x != x || y != y) {
    return S21_NAN;
  } else if (x > S21_DBL_MAX || -S21_DBL_MAX > x) {
    return S21_NAN;
  } else {
    int sign = x < 0 ? -1 : 1;
    long double res;
    x = s21_fabs(x);
    y = s21_fabs(y);
    if (y > x) {
      res = x * sign;
    } else {
      res = ((long double)x - s21_floor(x / y) * (long double)y) * sign;
    }
    return res;
  }
}
