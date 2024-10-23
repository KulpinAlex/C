#include "s21_mul.h"

#include "s21_another_functions.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  Init_s21_decimal(result);
  int sign_1 = getSign(value_1);
  int sign_2 = getSign(value_2);
  int exp_1 = get_exponent(value_1.bits[3]);
  int exp_2 = get_exponent(value_2.bits[3]);
  int exp_r = exp_1 + exp_2;
  if (exp_r > 255) error = 2;

  big_decimal big_result = s21_decimal_to_big_decimal(*result);
  if (error == 0) {
    set_exponent(&big_result.bits[6], exp_r);
    if (sign_1 != sign_2) {  // big decimal разных знаков
      SetBitFromUInt(&big_result.bits[6], 1, 31);
    } else {
      SetBitFromUInt(&big_result.bits[6], 0, 31);
    }
    s21_big_decimal_mul(value_1, value_2, &big_result);
    error = normalize_big_decimal(&big_result);
    big_decimal_to_s21_decimal(big_result, result);
    s21_optimize_dec(result);
  }
  if (s21_decimal_is_zero(result)) result->bits[3] = 0;
  if (error) {
    Init_s21_decimal(result);
  }
  return error;
}

void s21_big_decimal_mul(s21_decimal value_1, s21_decimal value_2,
                         big_decimal *result) {
  for (int i = 0; i < 3; i++) {
    unsigned long int tmp = 0;
    unsigned long int a = (unsigned long int)value_1.bits[i];
    for (int j = 0; j < 3; j++) {
      tmp += a * (unsigned long int)value_2.bits[j];
      tmp += (unsigned long int)result->bits[i + j];
      result->bits[i + j] = tmp & 0xffffffff;
      tmp >>= 32;
    }
    result->bits[i + 3] = tmp & 0xffffffff;
  }
}

int s21_div_body_int(s21_decimal *dec, int a) {
  unsigned long int b2 = (unsigned long int)dec->bits[2];
  unsigned long int b1 = (unsigned long int)dec->bits[1];
  unsigned long int b0 = (unsigned long int)dec->bits[0];

  b1 |= (b2 % a) << 32;
  b0 |= (b1 % a) << 32;
  int res = b0 % a;
  b2 /= a;
  b1 /= a;
  b0 /= a;

  dec->bits[2] = b2;
  dec->bits[1] = b1;
  dec->bits[0] = b0;

  return res;
}

void s21_optimize_dec(s21_decimal *dec) {
  int exp = get_exponent(dec->bits[3]);

  while (exp) {
    s21_decimal tmp = *dec;
    int ostatok = s21_div_body_int(dec, 10);
    if (ostatok == 0) {
      exp -= 1;
      set_exponent(&dec->bits[3], exp);
    } else {
      *dec = tmp;
      break;
    }
  }
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  if (value_2.bits[2] == 0 && value_2.bits[1] == 0 && value_2.bits[0] == 0)
    error = 3;
  Init_s21_decimal(result);
  int sign_1 = getSign(value_1);
  int sign_2 = getSign(value_2);
  int exp_2 = get_exponent(value_2.bits[3]);

  big_decimal big_result = s21_decimal_to_big_decimal(*result);
  big_decimal big_value_1 = s21_decimal_to_big_decimal(value_1);

  s21_maximize_big_dec(&big_value_1);
  int exp_big_value_1 = get_exponent(big_value_1.bits[6]);
  int exp_r = exp_big_value_1 - exp_2;
  if (exp_r < 0) error = 2;

  if (error == 0) {
    s21_big_decimal_div(big_value_1, value_2, &big_result);
    set_exponent(&big_result.bits[6], exp_r);
    if (sign_1 != sign_2) {  // big decimal разных знаков
      SetBitFromUInt(&big_result.bits[6], 1, 31);
    } else {
      SetBitFromUInt(&big_result.bits[6], 0, 31);
    }
    error = normalize_big_decimal(&big_result);
    big_decimal_to_s21_decimal(big_result, result);
    s21_optimize_dec(result);
  }
  if (error) {
    Init_s21_decimal(result);
  }
  return error;
}

void s21_big_decimal_div(big_decimal value_1, s21_decimal value_2,
                         big_decimal *result) {
  while (is_possible_to_div(value_1, value_2)) {
    big_decimal value_2_tmp = s21_decimal_to_big_decimal(value_2);
    big_decimal the_bit = {{1, 0, 0, 0, 0, 0, 0}};
    while (1) {
      big_decimal value_2_prev = value_2_tmp;
      int overflow = big_decimal_shift_left(&value_2_tmp);
      if (overflow || is_less_body(value_1, value_2_tmp)) {
        big_decimal tmp;
        s21_big_decimal_sub(value_1, value_2_prev, &tmp);
        value_1 = tmp;

        s21_big_decimal_add(*result, the_bit, &tmp);
        *result = tmp;
        break;
      }
      big_decimal_shift_left(&the_bit);
    }
  }
}

int is_possible_to_div(big_decimal a, s21_decimal b) {
  int flag = 1;
  if (a.bits[5] == 0 && a.bits[4] == 0 && a.bits[3] == 0) {
    for (int i = 2; i >= 0; i--) {
      if (a.bits[i] != b.bits[i]) {
        if (a.bits[i] < b.bits[i]) {
          flag = 0;
        }
        break;
      }
    }
  }
  return flag;
}

int is_less_body(big_decimal big_value_1, big_decimal big_value_2) {
  int result = 0;
  for (int i = 5; i >= 0; i--) {
    if (big_value_1.bits[i] != big_value_2.bits[i]) {
      if (big_value_1.bits[i] > big_value_2.bits[i]) {
        result = 0;
      } else {
        result = 1;
      }
      break;
    }
  }
  return result;
}

void s21_maximize_big_dec(big_decimal *dec) {
  int exp = get_exponent(dec->bits[6]);

  while (exp < 255) {
    big_decimal tmp = *dec;
    int overflows = s21_big_decimal_scale_ten(dec);
    if (overflows == 0) {
      exp += 1;
      set_exponent(&dec->bits[6], exp);
    } else {
      *dec = tmp;
      break;
    }
  }
}

int s21_mod_help(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result) {
  int err = 0;
  Init_s21_decimal(result);
  int check1 = s21_decimal_is_zero(&value_2);
  int check2 = s21_decimal_is_zero(&value_1);
  s21_decimal temp_result = {0};
  if (check1) {
    err = 3;
  } else if (!check2) {
    if (!s21_is_less_or_equal(value_1, value_2)) {
      err = s21_div(value_1, value_2, &temp_result);
      s21_truncate(temp_result, &temp_result);
      if (!err) {
        err = s21_mul(temp_result, value_2, &temp_result);
        if (!err) {
          err = s21_sub(value_1, temp_result, result);
        } else {
          s21_sub(value_1, value_2, &value_1);
          err = s21_mod_help(value_1, value_2, result);
        }
      } else {
        int exp = get_exponent(value_2.bits[3]);
        if (exp > 0) {
          big_decimal big_value_2 = s21_decimal_to_big_decimal(value_2);
          s21_big_decimal_scale_ten(&big_value_2);
          exp--;
          set_exponent(&big_value_2.bits[6], exp);
          normalize_big_decimal(&big_value_2);
          big_decimal_to_s21_decimal(big_value_2, &value_2);
          s21_mod_help(value_1, value_2, result);
        }
      }
    } else {
      for (int i = 0; i < 4; i++) {
        result->bits[i] = value_1.bits[i];
      }
    }
  }
  return err;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  SetBitFromUInt(&value_2.bits[3], 0, 31);
  int sign = getSign(value_1);
  SetBitFromUInt(&value_1.bits[3], 0, 31);
  err = s21_mod_help(value_1, value_2, result);
  if (!err) {
    while (s21_is_less_or_equal(value_2, *result)) {
      err = s21_mod_help(*result, value_2, result);
    }
  }
  if (sign) SetBitFromUInt(&result->bits[3], sign, 31);
  if (s21_decimal_is_zero(result)) result->bits[3] = 0;
  if (err) {
    Init_s21_decimal(result);
  }
  return err;
}
