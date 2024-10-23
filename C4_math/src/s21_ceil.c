#include "s21_math.h"

long double s21_ceil(double x) {
  long double res = 0.0;
  if (!(x != x)) {
    if (!(x > S21_DBL_MAX || -S21_DBL_MAX > x)) {
      int sign = x < 0 ? 1 : 0;
      res = (long long int)x;
      if ((x - res) > 0 && !sign) {
        res += 1;
      }
    } else {
      res = x > 0 ? S21_INF : S21_NINF;
    }
  } else {
    res = S21_NAN;
  }
  return res;
}