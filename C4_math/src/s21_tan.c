#include "s21_math.h"

long double s21_tan(double x) {
  long double sin = s21_sin(x);
  long double cos = s21_cos(x);
  return sin / cos;
}
