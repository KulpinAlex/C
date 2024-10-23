#include "s21_decimal.h"

#include "s21_another_functions.h"

#define PRECISION 7  // get # of digits from float (up to 99)

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  Init_s21_decimal(dst);
  int float2dec_error = 0;
  if (signbit(src)) {
    SetBit(dst, 127);
  }
  if (src != 0) {
    float2dec_error = CheckInfNanFloat(src);
    if (!float2dec_error) {
      float2dec_error = CheckFloatRange(src);
      if (!float2dec_error) {
        if (GetBit(*dst, 127)) {
          src = fabs(src);
        }
        char format[6] = "";
        sprintf(format, "%%.%de", PRECISION - 1);
        char *str_float = (char *)calloc(PRECISION + 6, sizeof(char));
        if (str_float == NULL) {
          fprintf(stderr,
                  "Can't allocate %zu bytes in s21_from_float_to_decimal()\n",
                  (PRECISION + 6) * sizeof(char));
          float2dec_error = -100;
        } else {
          sprintf(str_float, format, src);
          int exp_int = GetExpFromStrFloat(str_float);
          str_float[PRECISION + 1] = '\0';
          GetRidOfDot(str_float);
          if (exp_int < 0) {
            NormalizeStrFloat(str_float, &exp_int);
          }
          if (exp_int < -28) {
            float2dec_error = 3;
          } else {
            if (exp_int > 0) {
              float2dec_error = GetRidOfExp(&str_float, exp_int);
            }
            if (!float2dec_error) {
              float2dec_error = StrFloat2Dec(str_float, exp_int, dst);
            }
          }
          free(str_float);
        }
      }
    }
  }
  if (float2dec_error != -100) {
    SetValueType(dst, float2dec_error);
  }
  if (float2dec_error) {
    float2dec_error = 1;
  }
  return float2dec_error;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int dec2float_error = 0;
  if (src.value_type != 0) {
    dec2float_error = 1;
  } else {
    double res = 0.0;
    for (int i = 0; i < 96; ++i) {
      if (GetBit(src, i)) {
        res += pow(2, i);
      }
    }
    if (GetBit(src, 127)) {
      res *= (-1);
      src.bits[3] &= ~(1u << 31);
    }
    int scale = (src.bits[3] >> 16);
    for (int i = scale; i > 0; --i) {
      res /= 10.0;
    }
    *dst = (float)res;
  }

  return dec2float_error;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int int2dec_error = 0;
  Init_s21_decimal(dst);
  if (src < 0) {
    SetBit(dst, 127);
    dst->bits[0] = -src;
  } else {
    dst->bits[0] = src;
  }
  return int2dec_error;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int dec2int_error = 0;
  if (src.value_type != 0) {
    dec2int_error = 1;
  } else {
    s21_decimal src_trunc;
    Init_s21_decimal(&src_trunc);
    s21_truncate(src, &src_trunc);
    bool sign = 0;
    if (GetBit(src_trunc, 127)) {
      sign = 1;
      src_trunc.bits[3] &= ~(1u << 31);
    }
    if (src_trunc.bits[3] != 0 || src_trunc.bits[2] != 0 ||
        src_trunc.bits[1] != 0 || src_trunc.bits[0] > 2147483647u + sign) {
      dec2int_error = 1;
    } else {
      *dst = src_trunc.bits[0];
      if (sign) {
        *dst *= -1;
      }
    }
  }
  return dec2int_error;
}

void SetValueType(s21_decimal *dst, int float2dec_error) {
  switch (float2dec_error) {
    case 0:
    case 3:
    case 4:
      dst->value_type = 0;
      break;
    case -1:
    case 1:
      dst->value_type = 1;
      break;
    case -2:
    case 2:
      dst->value_type = 2;
      break;
  }
}

int CheckInfNanFloat(float src) {
  int inf_nan = 0;
  if (isnan(src)) {
    inf_nan = 2;
  } else if (isinf(src)) {
    inf_nan = 1;
  }
  if (signbit(src)) {
    inf_nan *= -1;
  }
  return inf_nan;
}

int CheckFloatRange(float src) {
  int out_of_range = 0;
  float_cast d = {.f = src};
  int e = d.parts.exponent;
  e -= 127;
  if (e < -95 || e > 95) {
    out_of_range = 4;
  }
  return out_of_range;
}

int GetExpFromStrFloat(char *str_float) {
  char str_exp_int[4] = "";
  str_exp_int[0] = str_float[PRECISION + 2];
  str_exp_int[1] = str_float[PRECISION + 3];
  str_exp_int[2] = str_float[PRECISION + 4];
  int exp_int = -(PRECISION - 1) + strtol(str_exp_int, NULL, 10);
  return exp_int;
}

void GetRidOfDot(char *str_float) {
  for (int i = 1; i <= PRECISION; ++i) {
    str_float[i] = str_float[i + 1];
  }
}

int GetRidOfExp(char **str_float, int exp_int) {
  int float2dec_error = 0;
  size_t len = strlen(*str_float);
  len += exp_int;
  char *tmp = realloc(*str_float, len + 1);
  if (tmp == NULL) {
    fprintf(stderr, "Can't allocate %zu bytes in GetRidOfExp()\n",
            (PRECISION + exp_int) * sizeof(char));
    float2dec_error = -100;
  } else {
    *str_float = tmp;
    strncat(*str_float, "00000000000000000000000000000", exp_int);
  }
  return float2dec_error;
}

void NormalizeStrFloat(char *str_float, int *exp_int) {
  for (int i = PRECISION - 1; i > 0; --i) {
    if (str_float[i] == '0' || *exp_int < -28) {
      str_float[i] = '\0';
      ++(*exp_int);
    } else {
      break;
    }
  }
}

void Init_s21_decimal(s21_decimal *dst) {
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
  dst->value_type = S21_NORMAL_VALUE;
}

int GetBitFromUInt(unsigned int number, int position) {
  return ((number & (1 << position)) != 0);
}

bool GetBit(s21_decimal dst, unsigned int index) {
  return (dst.bits[index / 32] & (1u << index % 32));
}

void SetBit(s21_decimal *dst, unsigned int index) {
  dst->bits[index / 32] |= (1u << index % 32);
}

int StrFloat2Dec(char *str_float, int exp_int, s21_decimal *dst) {
  int float2dec_error = 0;
  char *str_bin_number = (char *)calloc(97, sizeof(char));
  if (str_float == NULL) {
    fprintf(stderr, "Can't allocate %zu bytes in StrFloat2Dec()\n",
            (PRECISION + 6) * sizeof(char));
    float2dec_error = -100;
  } else {
    memset(str_bin_number, '0', 96);
    int digits = StrNum2StrBin(str_float, str_bin_number);
    StrBin2Dec(str_bin_number, digits, dst);
    free(str_bin_number);
    if (exp_int < 0) {
      exp_int = abs(exp_int);
      for (int i = 7, j = 119; i >= 0; --i, --j) {
        if (GetBitFromUInt(exp_int, i) == 1) {
          SetBit(dst, j);
        }
      }
    }
  }
  return float2dec_error;
}

int StrNum2StrBin(char *str_number, char *str_bin_number) {
  int digits = 0;
  size_t len = strlen(str_number);
  for (int j = 0; memcmp(str_number, "00000000000000000000000000000", len) != 0;
       ++j) {
    int remain = 0;
    for (size_t i = 0; i < len; ++i) {
      int tmp = 0;
      tmp = (remain + (str_number[i] - 48));
      if (tmp % 2 != 0) {
        remain = 10;
      } else {
        remain = 0;
      }
      str_number[i] = tmp / 2 + '0';
    }
    if (remain) {
      str_bin_number[95 - j] = '1';
    } else {
      str_bin_number[95 - j] = '0';
    }
    ++digits;
  }
  return digits;
}

void StrBin2Dec(char *str_bin_number, int digits, s21_decimal *dst) {
  for (int i = 0; i < digits; ++i) {
    if (str_bin_number[95 - i] == '1') {
      SetBit(dst, i);
    }
  }
}

int s21_decimal_is_zero(s21_decimal *dec) {
  // 1 true zero, 0 not zero
  return ((dec->bits[0] == 0) && (dec->bits[1] == 0) && (dec->bits[2] == 0));
}
