#include "s21_math.h"

long double s21_atan(double x) {
  long double a = x;
  long double sum = 0.0;
  int tmp = 1;
  int tmp_m = 1;
  long double tmp_Pi2 = 0.0;
  if (x > 1.0) {
    x = 1.0 / x;
    tmp_Pi2 = S21_PI / 2.0;
    tmp = -1;
    a = x;
  }
  if (x < -1.0) {
    x = -1.0 / x;
    tmp_Pi2 = S21_PI / 2.0;
    tmp = -1;
    a = x;
    tmp_m = -1;
  }
  for (int i = 0; i < 1000000; i++) {
    sum += a;
    a *= (-(2 * i + 1) * x * x) / (2 * i + 3);
  }

  return tmp_m * (tmp_Pi2 + sum * tmp);
}
