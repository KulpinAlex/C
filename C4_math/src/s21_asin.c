#include "s21_math.h"

long double s21_asin(double x) {
  long double arcsin = 0.0;
  long double a = x;
  long double sum = 0.0;
  if (s21_fabs(x) <= 1.0) {
    if ((1.000000 + x < 0.000001) && x < 0.0000) {
      sum = -1.570796;
    }
    if ((1.000000 - x < 0.000001) && x > 0.0000) {
      sum = 1.570796;
    }
    if (x > -1.0 && x < 1.0) {
      for (int i = 0; i < 100000; i++) {
        sum += a;
        a *= (((2 * i + 1) * (2 * i + 1)) * x * x) /
             ((2 * ((i + 1) * (2 * i + 3))));
      }
    }
    arcsin = sum;
  } else {
    arcsin = S21_NAN;
  }
  return arcsin;
}
