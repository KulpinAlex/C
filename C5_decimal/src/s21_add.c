#include "s21_add.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  Init_s21_decimal(result);
  int sign_1 = getSign(value_1);
  int sign_2 = getSign(value_2);
  int exp_1 = get_exponent(value_1.bits[3]);
  int exp_2 = get_exponent(value_2.bits[3]);
  int exp_r = exp_1 > exp_2 ? exp_1 : exp_2;
  if (exp_r > 28) error = 2;
  big_decimal big_value_1 = s21_decimal_to_big_decimal(value_1);
  big_decimal big_value_2 = s21_decimal_to_big_decimal(value_2);
  big_decimal big_result = s21_decimal_to_big_decimal(*result);
  if (error == 0) {
    set_exponent(&big_result.bits[6], exp_r);
    int exp_sub = abs(exp_1 - exp_2);
    if (exp_sub) {
      if (exp_1 > exp_2) {
        for (int i = 1; i <= exp_sub; i++) {
          s21_big_decimal_scale_ten(&big_value_2);
        }
      } else {
        for (int i = 1; i <= exp_sub; i++) {
          s21_big_decimal_scale_ten(&big_value_1);
        }
      }
    }
    if (sign_1 == sign_2) {  // big decimal одного знака
      SetBitFromUInt(&big_result.bits[6], sign_1, 31);
      s21_big_decimal_add(big_value_1, big_value_2, &big_result);
    } else {  // big decimal разных знаков
      int point = max_of_big_decimal(big_value_1, big_value_2);
      if (point == 0) {
        for (int i = 0; i < 7; i++) {
          big_result.bits[i] = 0;
        }
      } else if (point == 1) {
        s21_big_decimal_sub(big_value_1, big_value_2, &big_result);
        if (sign_1) SetBitFromUInt(&big_result.bits[6], 1, 31);
      } else {
        s21_big_decimal_sub(big_value_2, big_value_1, &big_result);
        if (sign_2) SetBitFromUInt(&big_result.bits[6], 1, 31);
      }
    }
    error = normalize_big_decimal(&big_result);
    big_decimal_to_s21_decimal(big_result, result);
  }
  if (s21_decimal_is_zero(result)) result->bits[3] = 0;
  if (error) {
    Init_s21_decimal(result);
  }
  return error;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sign_2 = getSign(value_2);
  if (sign_2) {
    SetBitFromUInt(&value_2.bits[3], 0, 31);
  } else {
    SetBitFromUInt(&value_2.bits[3], 1, 31);
  }
  return s21_add(value_1, value_2, result);
}

int normalize_big_decimal(big_decimal *value) {
  int err = 0;
  int temp1 = 0;
  int temp2 = 0;
  int point1 = 0;
  int point2 = 0;
  int exp = get_exponent(value->bits[6]);
  int check_zero = s21_big_decimal_is_zero(value);
  while ((value->bits[5] != 0) || (value->bits[4] != 0) ||
         (value->bits[3] != 0) || exp > 28) {
    point2 = 1;
    if (exp > 0) {
      if (!temp2) {
        temp2 = temp1;
      } else {
        point1 = 1;
      }
      temp1 = big_decimal_div_10(value);
      exp--;
    } else {
      if (GetBitFromUInt(value->bits[6], 31)) {
        err = 2;
      } else {
        err = 1;
      }
      break;
    }
  }
  if (!check_zero) {
    check_zero = s21_big_decimal_is_zero(value);
    if (check_zero) err = 2;
  }

  if (!err) {
    set_exponent(&value->bits[6], exp);
    if (point2) {
      big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
      if (temp1 == 5) {
        if (!((!GetBitFromUInt(value->bits[0], 0)) && (temp2 == 0) &&
              (point1 == 0))) {
          s21_big_decimal_add(*value, one, value);
          err = normalize_big_decimal(value);
        }
      } else if (temp1 > 5) {
        s21_big_decimal_add(*value, one, value);
        err = normalize_big_decimal(value);
      }
    }
  }

  return err;
}

// 0 если одинаковые, 1 если value1 > value2, 2 если value1 < value2
int max_of_big_decimal(big_decimal value1, big_decimal value2) {
  int result = 0;

  for (int i = 5; i >= 0; i--) {
    if (value1.bits[i] > value2.bits[i]) {
      result = 1;
      break;
    }
    if (value1.bits[i] < value2.bits[i]) {
      result = 2;
      break;
    }
  }

  return result;
}

int getSign(s21_decimal value) { return (value.bits[3] >> 31) & 1u; }

int get_exponent(int value) {  // TODO use bits
  unsigned int result = 0;
  for (int i = 16, j = 0; i < 24; i++, j++) {
    if (GetBitFromUInt(value, i)) {
      SetBitFromUInt(&result, 1, j);
    }
  }
  return result;
}

void set_exponent(unsigned int *value, int exp) {  // TODO use bits
  for (int i = 16, j = 0; i < 24; i++, j++) {
    if (GetBitFromUInt(exp, j)) {
      SetBitFromUInt(value, 1, i);
    } else {
      SetBitFromUInt(value, 0, i);
    }
  }
}

void SetBitFromUInt(unsigned int *number, int bit, int position) {
  if (bit == 1) {
    *number |= (1U << (position));
  } else {
    *number &= ~(1UL << position);
  }
}

big_decimal s21_decimal_to_big_decimal(s21_decimal value) {
  big_decimal result = {{0, 0, 0, 0, 0, 0, 0}};
  for (int i = 0; i < 3; i++) {
    result.bits[i] = value.bits[i];
  }
  result.bits[6] = value.bits[3];
  return result;
}

void big_decimal_to_s21_decimal(big_decimal value1, s21_decimal *value2) {
  for (int i = 0; i < 3; i++) {
    value2->bits[i] = value1.bits[i];
  }
  value2->bits[3] = value1.bits[6];
}

int s21_big_decimal_add(big_decimal value_1, big_decimal value_2,
                        big_decimal *result) {
  unsigned long temp = 0;
  for (int i = 0; i < 6; i++) {
    temp += value_1.bits[i];
    temp += value_2.bits[i];
    result->bits[i] = temp & 0xffffffff;
    temp >>= 32;
  }
  return temp;
}

void s21_big_decimal_sub(big_decimal value_1, big_decimal value_2,
                         big_decimal *result) {
  s21_big_decimal_add(value_1, negtive_big_decimal(value_2), result);
}

big_decimal negtive_big_decimal(big_decimal value) {
  big_decimal result = {{0, 0, 0, 0, 0, 0, 0}};
  big_decimal temp = {{1, 0, 0, 0, 0, 0, 0}};
  for (int i = 0; i < 6; i++) {
    result.bits[i] = ~value.bits[i];
  }
  s21_big_decimal_add(result, temp, &result);
  return result;
}

int s21_big_decimal_scale_ten(big_decimal *value) {
  big_decimal temp_value1, temp_value2;
  temp_value1 = temp_value2 = *value;
  int overflows = big_decimal_shift_left_number(&temp_value1, 1);
  overflows += big_decimal_shift_left_number(&temp_value2, 3);
  overflows += s21_big_decimal_add(temp_value1, temp_value2, value);
  return overflows;
}

int big_decimal_shift_left(big_decimal *value) {
  int temp1 = 0;
  for (int i = 0; i < 6; i++) {
    int temp2 = GetBitFromUInt(value->bits[i], 31);
    value->bits[i] <<= 1;
    SetBitFromUInt(&value->bits[i], temp1, 0);
    temp1 = temp2;
  }
  return temp1;
}

int big_decimal_shift_left_number(big_decimal *value, int position) {
  int overflows = 0;
  if (position > 0) {
    for (int i = 1; i <= position; i++) {
      overflows += big_decimal_shift_left(value);
    }
  }
  return overflows;
}

void big_decimal_shift_right(big_decimal *value) {
  int temp1 = 0;
  for (int i = 5; i >= 0; i--) {
    int temp2 = GetBitFromUInt(value->bits[i], 0);
    value->bits[i] >>= 1;
    SetBitFromUInt(&value->bits[i], temp1, 31);
    temp1 = temp2;
  }
}

int big_decimal_div_10(big_decimal *value) {
  int result = 0;
  big_decimal decimal_five = {{5, 0, 0, 0, 0, 0, 0}};
  big_decimal temp2 = {{0, 0, 0, 0, 0, 0, 0}};
  big_decimal temp1 = {{0, 0, 0, 0, 0, 0, 0}};
  int sign = GetBitFromUInt(value->bits[6], 31);
  big_decimal_copy(&temp1, *value);
  temp1.bits[6] = 0;
  big_decimal_shift_right(&temp1);  //делим на 2
  int i = 5;
  int j = 31;
  for (; i >= 0; i--) {
    j = 31;
    for (; j >= 0; j--) {
      if (GetBitFromUInt(temp1.bits[i], j)) break;
    }
    if (GetBitFromUInt(temp1.bits[i], j)) break;
  }
  big_decimal_shift_left_number(&decimal_five, i * 32 + j - 2);
  for (int h = 0; h < i * 32 + j - 1; h++) {
    int check = max_of_big_decimal(temp1, decimal_five);
    if ((check == 1) || (check == 0)) {
      s21_big_decimal_sub(temp1, decimal_five, &temp1);
      SetBitFromUInt(&temp2.bits[0], 1, 0);
    }
    big_decimal_shift_right(&decimal_five);
    if (h != i * 32 + j - 2) big_decimal_shift_left(&temp2);
  }
  result = (temp1.bits[0] << 1) + GetBitFromUInt(value->bits[0], 0);
  SetBitFromUInt(&temp2.bits[6], sign, 31);
  big_decimal_copy(value, temp2);
  return result;
}

void big_decimal_copy(big_decimal *value1, big_decimal value2) {
  for (int i = 0; i < 7; i++) {
    value1->bits[i] = value2.bits[i];
  }
}

int s21_big_decimal_is_zero(big_decimal *dec) {
  // 1 true zero, 0 not zero
  return ((dec->bits[0] == 0) && (dec->bits[1] == 0) && (dec->bits[2] == 0) &&
          (dec->bits[3] == 0) && (dec->bits[4] == 0) && (dec->bits[5] == 0));
}