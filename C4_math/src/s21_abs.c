#include "s21_math.h"

int s21_abs(int x) { return x > 0 ? x : -x; }

long double s21_fabs(double x) {
  long double res = 0.0;
  if (!(x != x)) {
    if (!(x > S21_DBL_MAX || -S21_DBL_MAX > x)) {
      res = x < 0 ? -x : x;
    } else {
      res = S21_INF;
    }
  } else {
    res = S21_NAN;
  }
  return res;
}