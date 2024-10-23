#include "s21_math.h"

long double s21_floor(double x) {
  if (x != x) {
    return S21_NAN;
  } else if (x > S21_DBL_MAX || -S21_DBL_MAX > x) {
    return x > 0 ? S21_INF : S21_NINF;
  } else {
    int sign = x < 0 ? 1 : 0;
    long double res;
    res = (long long int)x;
    if ((res - x) > 0 && sign) {
      res -= 1;
    }
    return res;
  }
}
