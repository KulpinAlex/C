#include "s21_math.h"

long double s21_sin(double x) {
  long double a = 0.0;
  long double sum = 0.0;
  x = s21_fmod(x, S21_PI * 2);
  a = x * x * x / 6 * (-1);
  sum = x;
  for (int i = 4; i < 100000; i += 2) {
    sum += a;
    a *= (-1) * x * x / ((i + 1) * (i));
  }
  return sum;
}
