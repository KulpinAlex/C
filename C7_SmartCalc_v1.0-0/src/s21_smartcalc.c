#include "s21_smartcalc.h"

int s21_smartcalc(char *str, double x, double *answer) {
  // char *str = "asin(0.3)-acos(0.3)*tan(0.3/atan(0.3))";
  //  char *str = "";
  int result = 0;
  if (strlen(str) != 0) {
    if (!(strlen(str) > 255)) {
      result = s21_check_str(str);

      if (!result) {
        lin_list *output = (lin_list *)calloc(1, sizeof(lin_list));
        if (output) {
          list *stack = NULL;
          output->first = NULL;
          output->last = NULL;
          result = s21_parser(str, &output);
          if (!result) {
            result = s21_calculate(&output, x, answer);
            s21_free_list(&stack);
          }
          s21_free_lin_list(&output);
        } else {
          result = 1;
        }
      }
    } else {
      result = 1;
    }
  }
  return result;
}

int s21_parser(char *str, lin_list **output) {
  int result = 0;
  char new_str[1600] = "";
  Data data = {0, 0, 0};
  list *stack = NULL;
  s21_clear_space(str, new_str);
  while (*new_str != '\0' && !result) {
    int size = strlen(new_str);
    char ch = *new_str;
    char *temp;
    if (ch >= '0' && ch <= '9') {
      double d = strtod(new_str, &temp);
      data = (Data){d, type_num, p_num_or_x};
      result = s21_push_lin_list(output, data);
    } else {
      temp = new_str;
      result = s21_parser_switch(output, ch, &stack, &temp);
    }
    memcpy(new_str, temp, size * sizeof(char));
  }
  while (stack && !result) {
    result = s21_pop_list(&stack, &data);
    if (!result) {
      result = s21_push_lin_list(output, data);
    }
  }
  s21_free_list(&stack);
  return result;
}

int s21_parser_switch(lin_list **head, char ch, list **stack, char **temp) {
  int result = 0;
  int shift = 0;
  Data data;
  switch (ch) {
    case 'x':
      data = (Data){0, type_x, p_num_or_x};
      result = s21_push_lin_list(head, data);
      shift = 1;
      break;
    case '-':
      data = (Data){0, type_sub, p_sum_sub};
      result = s21_check_switch(head, stack, &data);
      shift = 1;
      break;
    case '+':
      data = (Data){0, type_sum, p_sum_sub};
      result = s21_check_switch(head, stack, &data);

      shift = 1;
      break;
    case '(':
      data = (Data){0, type_bkt_open, p_bkt};
      result = s21_check_switch(head, stack, &data);
      shift = 1;
      break;
    case ')':
      data = (Data){0, type_bkt_close, p_bkt};
      while (!result && (*stack) && (*stack)->Data.type != type_bkt_open) {
        result = s21_pop_list(stack, &data);
        if (!result) {
          result = s21_push_lin_list(head, data);
        }
      }
      result = s21_pop_list(stack, &data);
      shift = 1;
      break;
    case '*':
      data = (Data){0, type_mul, p_mul_div_mod};
      result = s21_check_switch(head, stack, &data);
      shift = 1;
      break;
    case '/':
      data = (Data){0, type_div, p_mul_div_mod};
      result = s21_check_switch(head, stack, &data);
      shift = 1;
      break;
    case '^':
      data = (Data){0, type_pow, p_pow};
      result = s21_check_switch(head, stack, &data);
      shift = 1;
      break;
    case 'm':
      data = (Data){0, type_mod, p_mul_div_mod};
      result = s21_check_switch(head, stack, &data);
      shift = 3;
      break;
    case 'c':
      data = (Data){0, type_cos, p_func};
      result = s21_check_switch(head, stack, &data);
      shift = 3;
      break;
    case 's':
      s21_data_ch_s(&data, temp, &shift);
      result = s21_check_switch(head, stack, &data);
      break;
    case 't':
      data = (Data){0, type_tan, p_func};
      result = s21_check_switch(head, stack, &data);
      shift = 3;
      break;
    case 'a':
      s21_data_ch_a(&data, temp, &shift);
      result = s21_check_switch(head, stack, &data);
      shift = 4;
      break;
    case 'l':
      s21_data_ch_l(&data, temp, &shift);
      result = s21_check_switch(head, stack, &data);
      break;
    default:
      result = 1;
      break;
  }
  (*temp) = (*temp) + shift;
  return result;
}

void s21_data_ch_a(Data *data, char **temp, int *shift) {
  if (*((*temp) + 1) == 's') {
    *data = (Data){0, type_asin, p_func};
    *shift = 4;
  } else if (*((*temp) + 1) == 'c') {
    *data = (Data){0, type_acos, p_func};
    *shift = 4;
  } else {
    *data = (Data){0, type_atan, p_func};
    *shift = 4;
  }
}

void s21_data_ch_l(Data *data, char **temp, int *shift) {
  if (*((*temp) + 1) == 'n') {
    *data = (Data){0, type_ln, p_func};
    *shift = 2;
  } else {
    *data = (Data){0, type_log, p_func};
    *shift = 3;
  }
}

void s21_data_ch_s(Data *data, char **temp, int *shift) {
  if (*((*temp) + 1) == 'i') {
    *data = (Data){0, type_sin, p_func};
    *shift = 3;
  } else {
    *data = (Data){0, type_sqrt, p_func};
    *shift = 4;
  }
}

void s21_clear_space(char *str, char *sdt) {
  int i, j;
  i = j = 0;
  while (*(str + i) != '\0') {
    if (*(str + i) != ' ') {
      *(sdt + j) = *(str + i);
      j++;
    }
    i++;
    *(sdt + j) = '\0';
  }
  char new_str[1600] = "";
  s21_check_unary(sdt, new_str);
  i = 0;
  while (*(new_str + i) != '\0') {
    *(sdt + i) = *(new_str + i);
    i++;
  }
  *(sdt + i) = '\0';
}

//+->(1)*, - -> (0-1)*
void s21_check_unary(char *str, char *sdt) {
  int i, j;
  i = j = 0;
  while (*(str + i) != '\0') {
    if (i == 0 && *(str + i) == '+') {
      *(sdt + j) = '(';
      *(sdt + j + 1) = '1';
      *(sdt + j + 2) = ')';
      *(sdt + j + 3) = '*';
      j += 4;

    } else if (i == 0 && *(str + i) == '-') {
      *(sdt + j) = '(';
      *(sdt + j + 1) = '0';
      *(sdt + j + 2) = '-';
      *(sdt + j + 3) = '1';
      *(sdt + j + 4) = ')';
      *(sdt + j + 5) = '*';
      j += 6;
    } else if (i != 0 &&
               !(*(str + i - 1) == ')' || *(str + i - 1) == 'x' ||
                 *(str + i - 1) == 'd' ||
                 (*(str + i - 1) >= '0' && *(str + i - 1) <= '9')) &&
               ((*(str + i + 1) >= '0' && *(str + i + 1) <= '9') ||
                *(str + i + 1) == 'x')) {
      if (*(str + i) == '+') {
        *(sdt + j) = '(';
        *(sdt + j + 1) = '1';
        *(sdt + j + 2) = ')';
        *(sdt + j + 3) = '*';
        j += 4;
      } else if (*(str + i) == '-') {
        *(sdt + j) = '(';
        *(sdt + j + 1) = '0';
        *(sdt + j + 2) = '-';
        *(sdt + j + 3) = '1';
        *(sdt + j + 4) = ')';
        *(sdt + j + 5) = '*';
        j += 6;
      } else {
        *(sdt + j) = *(str + i);
        j++;
      }
    } else {
      *(sdt + j) = *(str + i);
      j++;
    }
    i++;
  }
}

int s21_check_str(char *str) {
  int result = 0;
  char new_str[1600] = "";
  int check_bkt = 0;
  char *check = "()+-*/^modsincostansqrtlnlogx";
  s21_clear_space(str, new_str);
  int size = strlen(new_str);
  int i = 0;
  while (*(new_str + i) != '\0') {
    if (*(new_str + i) == 'x') *(new_str + i) = '1';
    i++;
  }
  char point = -1;
  result = s21_check_num(str);

  while ((*new_str != '\0') && !result) {
    char *temp;
    strtod(new_str, &temp);
    char ch = *temp;
    i = 0;
    while (*(check + i) != '\0' && ch != '\0') {
      result = 1;
      if (*(check + i) == ch) {
        result = 0;
        break;
      }
      i++;
    }
    if (ch == '(' || ch == ')') {
      if (ch == '(')
        check_bkt++;
      else
        check_bkt--;
      result = s21_check_bkt_oc(temp, ch, point);
    }
    if (!result && *temp != '\0') {
      result = s21_check_func_inp(new_str, ch);
      if (!result) result = s21_check_ch_end(temp, ch);
    }
    if (result) break;
    if (ch == 's') {
      if (*(temp + 1) == 'i') {
        result = s21_check_ch(temp, "sin", 3);
        if (!result) temp += 2;
      } else {
        result = s21_check_ch(temp, "sqrt", 4);
        if (!result) temp += 3;
      }
    } else if (ch == 'l') {
      if (*(temp + 1) == 'n') {
        result = s21_check_ch(temp, "ln", 2);
        if (!result) temp += 1;
      } else {
        result = s21_check_ch(temp, "log", 3);
        if (!result) temp += 2;
      }
    } else if (ch == 'a') {
      result = s21_check_ch_a(temp);
      if (!result) temp += 3;
    } else if ((ch == 'c') || (ch == 'm') || (ch == 't')) {
      result = s21_check_ch_cmt(temp, ch);
      if (!result) temp += 2;
    }
    if (result || check_bkt < 0) break;
    temp++;
    memcpy(new_str, temp, size * sizeof(char));
    point = ch;
  }
  if (check_bkt) result = 1;
  return result;
}

int s21_check_num(char *str) {
  int result = 1;
  int i = 0;
  while (*(str + i) != '\0') {
    if ((*(str + i) >= '0' && *(str + i) <= '9') || *(str + i) == 'x') {
      result = 0;
      break;
    }
    i++;
  }
  return result;
}

int s21_check_ch(char *str, const char *dst, int size) {
  int result = 0;
  for (int i = 0; i < size; i++) {
    if (*(str + i) == '\0') {
      result = 1;
      break;
    }
  }
  if (!result) {
    for (int i = 0; i < size; i++) {
      if (*(str + i) != dst[i]) {
        result = 1;
        break;
      }
    }
  }
  if (!result) {
    if (*(str + size) == '\0' || *(str + size) == '*' || *(str + size) == '/' ||
        *(str + size) == '^' || *(str + size) == 'm' || *(str + size) == ')' ||
        *(str + size) == '-' || *(str + size) == '+')
      result = 1;
  }
  return result;
}

int s21_check_ch_a(char *str) {
  int result = 0;
  if (*(str + 1) == 'c') {
    result = s21_check_ch(str, "acos", 4);
  } else if (*(str + 1) == 's') {
    result = s21_check_ch(str, "asin", 4);
  } else if (*(str + 1) == 't') {
    result = s21_check_ch(str, "atan", 4);
  } else {
    result = 1;
  }
  return result;
}

int s21_check_ch_cmt(char *str, char ch) {
  int result = 0;
  if (ch == 'm') {
    result = s21_check_ch(str, "mod", 3);
  }
  if (ch == 'c') {
    result = s21_check_ch(str, "cos", 3);
  }
  if (ch == 't') {
    result = s21_check_ch(str, "tan", 3);
  }
  return result;
}

int s21_check_bkt_oc(char *str, char ch, char last) {
  int result = 0;
  if (ch == '(') {
    if (*(str + 1) == ')' || *(str + 1) == '^' || *(str + 1) == '/' ||
        *(str + 1) == '*' || *(str + 1) == 'm')
      result = 1;
    if (last != -1) {
      if (!(last == 'm' || last == '^' || last == '/' || last == '*' ||
            last == '-' || last == '+' || last == 'c' || last == 's' ||
            last == 't' || last == 'a' || last == 'l' || last == '('))
        result = 1;
    }
  } else if (*(str + 1) != '\0') {
    if (!(*(str + 1) == '^' || *(str + 1) == '/' || *(str + 1) == '*' ||
          *(str + 1) == '-' || *(str + 1) == '+' || *(str + 1) == ')' ||
          *(str + 1) == 'm'))
      result = 1;
  }
  return result;
}

int s21_check_func_inp(char *str, char ch) {
  int result = 0;
  if ((*str >= '0' && *str <= '9') ||
      ((*str == '+' || *str == '-') &&
       (*(str + 1) >= '0' && *(str + 1) <= '9'))) {
    if (!(ch == '+' || ch == '^' || ch == '/' || ch == '*' || ch == '-' ||
          ch == 'm' || ch == ')'))
      result = 1;
  }
  return result;
}

int s21_check_ch_end(char *str, char ch) {
  int result = 0;
  if (ch == '+' || ch == '^' || ch == '/' || ch == '*' || ch == '-') {
    if (*(str + 1) == ')' || *(str + 1) == '\0' || *(str + 1) == '*' ||
        *(str + 1) == '/' || *(str + 1) == '^' || *(str + 1) == 'm')
      result = 1;
  }
  return result;
}

int s21_push_list(list **head, Data data) {
  int res = 0;
  list *tmp = (list *)calloc(1, sizeof(list));
  if (tmp) {
    tmp->Data = data;
    tmp->next = *head;
    *head = tmp;
  } else {
    res = 1;
  }
  return res;
}

void s21_free_list(list **head) {
  if (*head) {
    list *tmp = (*head)->next;
    list *next = NULL;
    while (tmp && *head) {
      next = tmp->next;
      free(tmp);
      tmp = next;
    }
    free(*head);
    (*head) = NULL;
  }
}

int s21_push_lin_list(lin_list **head, Data data) {
  int res = 0;
  list *new = (list *)calloc(1, sizeof(list));
  if (new) {
    new->Data = data;
    if (!res) {
      if ((*head)->first) {
        new->next = NULL;
        (*head)->last->next = new;
      } else {
        new->next = (*head)->first;
        (*head)->first = new;
      }
      (*head)->last = new;
    }
  } else {
    res = 1;
  }
  return res;
}

void s21_free_lin_list(lin_list **head) {
  if (*head) {
    list *tmp = (*head)->first;
    list *next = NULL;
    while (tmp && *head) {
      next = tmp->next;
      free(tmp);
      tmp = next;
    }
    free(*head);
    (*head) = NULL;
  }
}

int s21_pop_lin_list(lin_list **head, Data *data) {
  int result = 0;

  if ((*head) && (*head)->first) {
    list *tmp = (*head)->first;
    *data = (*head)->first->Data;
    (*head)->first = (*head)->first->next;
    free(tmp);
  } else {
    result = 1;
  }
  return result;
}

int s21_pop_list(list **head, Data *data) {
  int result = 0;
  if (*head) {
    list *tmp = *head;
    *data = (*head)->Data;
    *head = (*head)->next;
    free(tmp);
  } else {
    result = 1;
  }
  return result;
}

int s21_check_switch(lin_list **head, list **stack, Data *data) {
  int result = 0;
  Data tmp = {0, 0, 0};
  if (!*stack || ((*stack)->Data.pr < data->pr) ||
      ((*stack)->Data.type == type_bkt_open && data->type == type_bkt_open)) {
    result = s21_push_list(stack, *data);
  } else {
    while ((*stack) && !result && (*stack)->Data.pr >= data->pr &&
           (*stack)->Data.type != type_bkt_open) {
      result = s21_pop_list(stack, &tmp);
      if (!result) {
        result = s21_push_lin_list(head, tmp);
      }
    }
    result = s21_push_list(stack, *data);
  }
  return result;
}

int s21_calculate(lin_list **output, double x, double *answer) {
  int result = 0;
  Data data = {0, 0, 0};
  Data val1 = {0, 0, 0};
  Data val2 = {0, 0, 0};
  list *stack = NULL;
  while (!result && (*output) && (*output)->first) {
    result = s21_pop_lin_list(output, &data);
    if (!result) {
      if (data.pr == p_num_or_x) {
        if (data.type == type_x) data.value = x;
        result = s21_push_list(&stack, data);
      } else if (data.pr == p_func) {
        result = s21_pop_list(&stack, &val2);
        if (!result) {
          result = s21_calc_function(&val2, data.type);
          if (!result && isfinite(val2.value)) {
            result = s21_push_list(&stack, val2);
          } else {
            result = 1;
          }
        }

      } else {
        result = s21_pop_list(&stack, &val2);
        if (!result) {
          result = s21_pop_list(&stack, &val1);
          if (!result) {
            result = s21_calc_binary(&val1, &val2, data.type);
            if (!result && isfinite(val1.value)) {
              result = s21_push_list(&stack, val1);
            } else {
              result = 1;
            }
          }
        }
      }
    }
  }
  if (!result) {
    result = s21_pop_list(&stack, &data);
    *answer = data.value;
  }
  s21_free_list(&stack);
  return result;
}

int s21_calc_binary(Data *val1, Data *val2, int type) {
  int result = 0;
  switch (type) {
    case 4:  // +
      val1->value += val2->value;
      break;
    case 5:  // -
      val1->value -= val2->value;
      break;
    case 6:  // *
      val1->value *= val2->value;
      break;
    case 7:  // /
      if (fabs(val2->value) < 1e-10) {
        result = 1;
      } else {
        val1->value /= val2->value;
      }
      break;
    case 8:  // ^
      val1->value = pow(val1->value, val2->value);
      break;
    case 9:  // mod
      if (fabs(val2->value) < 1e-10) {
        result = 1;
      } else {
        val1->value = fmod(val1->value, val2->value);
      }
      break;
    default:
      result = 1;
      break;
  }
  return result;
}

int s21_calc_function(Data *val1, int type) {
  int result = 0;
  switch (type) {
    case 10:  // cosx
      val1->value = cos(val1->value);
      break;
    case 11:  // sinx
      val1->value = sin(val1->value);
      break;
    case 12:  // tanx
      if (fabs((fmod(val1->value, (double)S21_PI) - (double)S21_PI / 2)) <
          1e-16) {
        result = 1;
      } else {
        val1->value = tan(val1->value);
      }
      break;
    case 13:  // acosx
      if (fabs(val1->value) > 1.0) {
        result = 1;
      } else {
        val1->value = acos(val1->value);
      }
      break;
    case 14:  // asinx
      if (fabs(val1->value) > 1.0) {
        result = 1;
      } else {
        val1->value = asin(val1->value);
      }
      break;
    case 15:  // atanx
      val1->value = atan(val1->value);
      break;
    case 16:  // sqrt
      if (val1->value < 0.0) {
        result = 1;
      } else {
        val1->value = sqrt(val1->value);
      }
      break;
    case 17:  // ln
      if (val1->value < 0.0 || fabs(val1->value) < 1e-323) {
        result = 1;
      } else {
        val1->value = log(val1->value);
      }
      break;
    case 18:  // log
      if (val1->value < 0.0 || fabs(val1->value) < 1e-323) {
        result = 1;
      } else {
        val1->value = log10(val1->value);
      }
      break;
    default:
      result = 1;
      break;
  }
  return result;
}

/*
void s21_print_lin_list(lin_list **output) {
  printf("\n");
  Data data;
  int result = 0;
  while (!result) {
    result = s21_pop_lin_list(output, &data);

    if (!result) {
      switch (data.type) {
        case type_num:
          printf(" %.1f ", data.value);
          break;
        case type_x:
          printf("x");
          break;
        case type_bkt_open:
          printf("(");
          break;
        case type_bkt_close:
          printf(")");
          break;
        case type_sum:
          printf("+");
          break;
        case type_sub:
          printf("-");
          break;
        case type_mul:
          printf("*");
          break;
        case type_div:
          printf("/");
          break;
        case type_pow:
          printf("^");
          break;
        case type_mod:
          printf("mod");
          break;
        case type_cos:
          printf("cos");
          break;
        case type_sin:
          printf("sin");
          break;
        case type_tan:
          printf("tan");
          break;
        case type_acos:
          printf("acos");
          break;
        case type_asin:
          printf("asin");
          break;
        case type_atan:
          printf("atan");
          break;
        case type_sqrt:
          printf("sqrt");
          break;
        case type_ln:
          printf("ln");
          break;
        case type_log:
          printf("log");
          break;
        default:
          printf("-->error<<--");
          break;
      }
    }
  }
  printf("\n");
}
*/
