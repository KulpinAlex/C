#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  S21_NORMAL_VALUE = 0,
  S21_INFINITY = 1,
  S21_NAN = 2
} value_type_t;

// +-2 - if +-NAN,
// +-1 - if +-INF,
// 0 otherwise
typedef struct {
  unsigned int bits[4];
  value_type_t value_type;
} s21_decimal;

typedef union {
  float f;
  struct {
    unsigned int mantisa : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
  } parts;
} float_cast;

// 1 error
// 0 otherwise
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
// +-2 if +-NAN, +-1 if +-INF, 0 otherwise
int CheckInfNanFloat(float src);
// 0 if out of range (|x| > 79,228,162,514,264,337,593,543,950,335), 1 otherwise
int CheckFloatRange(float src);
int GetExpFromStrFloat(char *str_float);
void GetRidOfDot(char *str_float);
int GetRidOfExp(char **str_float, int exp_int);
void NormalizeStrFloat(char *str_float, int *exp_int);

// helpful functions
void Init_s21_decimal(s21_decimal *dst);
int GetBitFromUInt(unsigned int number, int position);
bool GetBit(s21_decimal dst, unsigned int index);
void SetBit(s21_decimal *dst, unsigned int index);
int StrFloat2Dec(char *str_float, int exp_int, s21_decimal *dst);
int StrNum2StrBin(char *str_number, char *str_bin_number);
void StrBin2Dec(char *str_bin_number, int digits, s21_decimal *dst);
void SetValueType(s21_decimal *dst, int float2dec_error);
int s21_decimal_is_zero(s21_decimal *dec);

#endif  // SRC_S21_DECIMAL_H_
