// gcc -g test2.c -o test `pkg-config --cflags --libs check`
// gcc -Wall -Wextra -Werror -std=c11 -lm -lcheck -lpthread

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char test_pack_cat[12][30] = {
    " -b", " -e", " -n", " -s", " -t", " -v", "",
};

START_TEST(s21_cat_test_1) {
  char actual_1[100] = "cat";
  char expected_1[100] = "./s21_cat";
  system(strcat(strcat(actual_1, test_pack_cat[_i]),
                " test_file.txt > s21_original"));
  system(strcat(strcat(expected_1, test_pack_cat[_i]),
                " test_file.txt > s21_test"));
  system("diff -s s21_test s21_original > result");
  FILE *file = fopen("result", "r");
  if (file != NULL) {
    char result[100];
    fgets(result, sizeof(result), file);
    ck_assert_pstr_eq(result,
                      "Files s21_test and s21_original are identical\n");
    fclose(file);
  }
}

START_TEST(s21_cat_test_2) {
  for (int t = 0; t < 12; t++) {
    char actual_1[100] = "cat ";
    char expected_1[100] = "./s21_cat ";
    system(strcat(strcat(strcat(actual_1, test_pack_cat[_i]), test_pack_cat[t]),
                  " test_file.txt > s21_original"));

    system(
        strcat(strcat(strcat(expected_1, test_pack_cat[_i]), test_pack_cat[t]),
               " test_file.txt > s21_test"));

    system("diff -s s21_test s21_original > result");
    FILE *file = fopen("result", "r");
    if (file != NULL) {
      char result[100];
      fgets(result, sizeof(result), file);
      ck_assert_pstr_eq(result,
                        "Files s21_test and s21_original are identical\n");
      fclose(file);
    }
  }
}

int main() {
  Suite *s = suite_create(
      "s21_cat test");  // устанавливает название всего набора тестов
  SRunner *sr =
      srunner_create(s);  // подключает набор к утилите для запуска тестов
  srunner_set_fork_status(sr, CK_NOFORK);  // настраивает возможность отключить
                                           // запуск тестов в несколько потоков

  TCase *tc_funcs =
      tcase_create("functions case");  // тесты могут быть разбиты на группы
  suite_add_tcase(s, tc_funcs);  // подключает группу тестов к набору
  tcase_set_timeout(tc_funcs,
                    10);  // устанавливает максимальное время исполнения

  int size = sizeof(test_pack_cat) / sizeof(test_pack_cat[0]);
  tcase_add_loop_test(tc_funcs, s21_cat_test_1, 0, size);
  tcase_add_loop_test(tc_funcs, s21_cat_test_2, 0, size);

  srunner_run_all(sr, CK_ENV);  // запускает тесты
  srunner_free(sr);             // освобождает ресурсы
  return 0;                     // this is the way
}
