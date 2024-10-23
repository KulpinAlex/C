#include "s21_math.h"

long double s21_acos(double x) {
  long double sum = 0.0;
  sum = s21_asin(x);
  sum = S21_PI / 2.0 - sum;
  return sum;
}
