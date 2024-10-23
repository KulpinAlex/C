#include "s21_math.h"

long double s21_cos(double x) {
  long double sum = 1.0;
  long double a = 1.0;
  x = s21_fmod(x, S21_PI * 2);
  for (int i = 1; i < 100000; i++) {
    a *= (-1) * x * x / ((2 * i - 1) * (2 * i));
    sum += a;
  }
  return sum;
}
