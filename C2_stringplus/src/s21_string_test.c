// gcc s21_strings_test.c -lcheck -o s21_strings_test && ./s21_strings_test

#include <check.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "s21_string.h"

#define MESSAGE_S "ERROR test %i ('%s'), we expected '%s', but got      '%s'"
#define MESSAGE_D "ERROR test %i, we expected '%i', but got      '%i'"

char actual_buf[200], expected_buf[200];

// s21_memchr

struct test_pack_memchr_struct {
    char str[20];
    char ch;
    char n;
};
struct test_pack_memchr_struct test_pack_memchr[] = {
    { "qwerty",   'q', 6 },
    { "qwerty",   'q', 7 },
    { "qwerty",   'q', 0 },
    { "",         'q', 6 },
    { "qwerty",   'Q', 3 },
    { "qwerty",   'y', 6 },
    { "qwerty",   'y', 5 },
    { "qwerty",   113, -6 },
    { "113werty", 113, 6 },
    { " \n",      ' ', 100 },
    { "qwerty\n", 10,  7 },
};

START_TEST(s21_memchr_test) {
    char *actual = s21_memchr(test_pack_memchr[_i].str, test_pack_memchr[_i].ch, test_pack_memchr[_i].n);
    char *expected = memchr(test_pack_memchr[_i].str, test_pack_memchr[_i].ch, test_pack_memchr[_i].n);
    ck_assert_pstr_eq(actual, expected);
}


// s21_memcmp

struct test_pack_memcmp_struct {
    char str1[20];
    char str2[20];
    char n;
};
struct test_pack_memcmp_struct test_pack_memcmp[] = {
    { "Hello World",   "Hello World",   10 },
    { "Hello World",   "Hello World",   100 },
    { "Hello Worl",    "Hello World",   11 },
    { "Hello World",   "Hello Worl",    11 },
    { "Hello\0 World", "Hell\0o World", 6 },
    { " ",             "",              1 },
    { "\0",            "\0",            4 },
    { "Hello World",   "Hello ",        4 },
    { " Hello!",       "",              0 },
    { " Hello!",       " Hello!",      -5 },
    { "\n",            "\n ",           0 },
};

START_TEST(s21_memcmp_test) {
    int actual = s21_memcmp(test_pack_memcmp[_i].str1, test_pack_memcmp[_i].str2, test_pack_memcmp[_i].n);
    int expected = memcmp(test_pack_memcmp[_i].str1, test_pack_memcmp[_i].str2, test_pack_memcmp[_i].n);
    ck_assert_int_eq(actual, expected);
}
END_TEST


// s21_memcpy

struct test_pack_memcpy_struct {
    char dest[10];
    char src[10];
    char n;
};
struct test_pack_memcpy_struct test_pack_memcpy[] = {
    { "",         "Hello",  5 },
    { "Hello",    " ",      3 },
    { "",         "",       2 },
    { "Hello",    "World!", 10 },
    { "\0",       "\n",     3 },
    { "Hello",    "World!", 0 },
    { "\0",       "\n",     0 },
    { "\0",       "\n",     100 },
    { "Wor\0ld!", "\n",     7 },
    { "\n",       "\n",     0 },
};

START_TEST(s21_memcpy_test) {
    char *actual = s21_memcpy(test_pack_memcpy[_i].dest, test_pack_memcpy[_i].src, test_pack_memcpy[_i].n);
    char *expected = memcpy(test_pack_memcpy[_i].dest, test_pack_memcpy[_i].src, test_pack_memcpy[_i].n);
    ck_assert_pstr_eq(actual, expected);
}
END_TEST


// s21_memmove

struct test_pack_memmove_struct {
    char dest[10];
    char src[10];
    char n;
};
struct test_pack_memmove_struct test_pack_memmove[] = {
    { "Hello",    "World",  3 },
    { "Hello",    " ",      3 },
    { "",         "",       2 },
    { "Hello",    "World!", 10 },
    { "\0",       "\n",     0 },
};

START_TEST(s21_memmove_test_1) {
    char *actual = s21_memmove(test_pack_memmove[_i].dest, test_pack_memmove[_i].src,
        test_pack_memmove[_i].n);
    char *expected = memmove(test_pack_memmove[_i].dest, test_pack_memmove[_i].src, test_pack_memmove[_i].n);
    ck_assert_pstr_eq(actual, expected);
}
END_TEST

START_TEST(s21_memmove_test_2) {
    char *actual = s21_memmove(test_pack_memmove[_i].dest, test_pack_memmove[_i].dest,
        test_pack_memmove[_i].n);
    char *expected = memmove(test_pack_memmove[_i].dest, test_pack_memmove[_i].dest, test_pack_memmove[_i].n);
    ck_assert_pstr_eq(actual, expected);
}
END_TEST


struct test_pack_memmove_struct3 {
    char actual[20];
    char expect[20];
    int  delta_dest;
    int  delta_src;
    int  n;
};
struct test_pack_memmove_struct3 test_pack_memmove3[] = {
    { "1234567890", "1234567890", 3, 0, 5 },  // "1234590"
    { "1234567890", "1234567890", 0, 3, 5 },  // "4567867890"
};

START_TEST(s21_memmove_test_3) {
    char *actual = s21_memmove(test_pack_memmove3[_i].actual + test_pack_memmove3[_i].delta_dest,
        test_pack_memmove3[_i].actual + test_pack_memmove3[_i].delta_src, test_pack_memmove3[_i].n);
    char *expected = memmove(test_pack_memmove3[_i].expect + test_pack_memmove3[_i].delta_dest,
        test_pack_memmove3[_i].expect + test_pack_memmove3[_i].delta_src, test_pack_memmove3[_i].n);
    ck_assert_pstr_eq(actual, expected);
}
END_TEST


// s21_memset

struct test_pack_memset_struct {
    char str1[20];
    char str2[20];
    char c;
    char n;
};
struct test_pack_memset_struct test_pack_memset[] = {
    { "Hello Word", "Hello Word", 'H', 5 },
    { "Hello WorD", "Hello WorD", '1', 9 },
    { " ", " ", 'A', 1 },
    { " \n", " \n", 'f', 10 },
    { " \n", " \n", '-', 3 },
    { "hello", "hello", '*', 0 },
    { "World", "World", '%', 6 },
    { "    ", "    ", '3', 100 },
    { "Wor\0ld", "Wor\0ld", 'K', 2 },
    { "Hello Word", "Hello Word", ' ', 5 },
    { "", "", 'y', 5 },
};

START_TEST(s21_memset_test) {
    char *actual = s21_memset(test_pack_memset[_i].str1, test_pack_memset[_i].c, test_pack_memset[_i].n);
    char *expected = memset(test_pack_memset[_i].str2, test_pack_memset[_i].c, test_pack_memset[_i].n);
    ck_assert_mem_eq(actual, expected, test_pack_memset[_i].n);
}
END_TEST


// s21_strcat
/*
Тесты:
две строки с символами
добавление строки с символами к пустой строке
добавление пустой строки к строке с символами
добавление пустой строки к пустой строке
проверка на спец символы
*/

struct test_pack_strcat_struct {
    char str1[20];
    char str2[20];
    char str3[20];
};
struct test_pack_strcat_struct test_pack_strcat[] = {
    { "Hello ", "Hello ", "World!" },
    { "",       "",       "World!" },
    { "Hello ", "Hello ", "" },
    { "",       "",       "" },
    { "\0",     "\0",     "\n" },
    { "\n",     "\n",     "\0" },
    { "\0",     "\0",     "\0" },
};

START_TEST(s21_strcat_test) {
    char* actual = s21_strcat(test_pack_strcat[_i].str1, test_pack_strcat[_i].str3);
    char* expected = strcat(test_pack_strcat[_i].str2, test_pack_strcat[_i].str3);
    ck_assert_msg(strcmp(actual, expected) == 0, MESSAGE_S, _i, "", expected, actual);
}
END_TEST


// s21_strncat
/*
Тесты:
две строки с символами, размер больше добавляемой строки
две строки с символами, размер меньше добавляемой строки
две строки с символами, размер больше добавляемой строки в которой есть символ конца строки
добавление пустой строки к пустой строке
две строки с символами, размер равен 0
*/

struct test_pack_strncat_struct {
    char str1[20];
    char str2[20];
    char str3[20];
    int  size;
};
struct test_pack_strncat_struct test_pack_strncat[] = {
    { "Hello ", "Hello ", "World!",   20 },
    { "Hello ", "Hello ", "World!",   3 },
    { "Hello ", "Hello ", "World!",   0 },
    { "Hello ", "Hello ", "Wor\0ld!", 20 },
    { "",       "",       "",         2 },
    { "",       "",       "",         1 },
    { "",       "",       "",         0 },
    //
    { "",       "",       "World!",   20 },
    { "Hello ", "Hello ", "",         20 },
    { "\0",     "\0",     "\n",       20 },
    { "\n",     "\n",     "\0",       20 },
    { "\0",     "\0",     "\0",       20 },
    { "\0",     "\0",     "\n",       1 },
    { "\n",     "\n",     "\0",       1 },
    { "\0",     "\0",     "\0",       1 },
    { "\0",     "\0",     "\n",       0 },
    { "\n",     "\n",     "\0",       0 },
    { "\0",     "\0",     "\0",       0 },
};

START_TEST(s21_strncat_test) {
    char* actual = s21_strncat(test_pack_strncat[_i].str1, test_pack_strncat[_i].str3,
        test_pack_strncat[_i].size);
    char* expected = strncat(test_pack_strncat[_i].str2, test_pack_strncat[_i].str3,
        test_pack_strncat[_i].size);
    ck_assert_msg(strcmp(actual, expected) == 0, MESSAGE_S, _i, "", expected, actual);
}
END_TEST


// s21_strchr
/*
Тесты:
символ в строке присутствует
символ в строке отсутствует
поиск символа конца строки
поиск символа переноса строки
поиск символа в пустой строке
поиск нсимвола конца строки в пустой строке
*/

struct test_pack_strchr_struct {
    char str[20];
    char ch;
};
struct test_pack_strchr_struct test_pack_strchr[] = {
    { "abcs7df", '7' },
    { "abcs7df", 'e' },
    { "abcs7df", '\0' },
    { "abcs7df", '\n' },
    { "",        '5' },
    { "",        '\0' },
};

START_TEST(s21_strchr_test) {
    char* actual = s21_strchr(test_pack_strchr[_i].str, test_pack_strchr[_i].ch);
    char* expected = strchr(test_pack_strchr[_i].str, test_pack_strchr[_i].ch);
    ck_assert_pstr_eq(actual, expected);
}
END_TEST


// s21_strcmp
/*
Тесты:
две разные строки одного размера
вторая строка больше первой
первая строка больше второй
первая строка пустая, вторая нет
вторая строка пустая, первая нет
две пустые строки
одинаковые строки
*/

struct test_pack_strcmp_struct {
    char str1[20];
    char str2[20];
};
struct test_pack_strcmp_struct test_pack_strcmp[] = {
    { "abc",  "bcd" },
    { "abc",  "abcd"},
    { "abcd", "abc" },
    { "",     "abc" },
    { "abc",  ""},
    { "",     ""},
    { "abc",  "abc"},
};

START_TEST(s21_strcmp_test) {
    int actual = s21_strcmp(test_pack_strcmp[_i].str1, test_pack_strcmp[_i].str2);
    int expected = strcmp(test_pack_strcmp[_i].str1, test_pack_strcmp[_i].str2);
    ck_assert_msg(actual == expected, MESSAGE_D, _i + 1, expected, actual);
}


// s21_strncmp
/*
Тесты:
две разные строки одного размера, size больше длинны строки
вторая строка больше первой, есть символ конца строки
строки одинаковой длинны, size меньше длинны строки
первая строка пустая, вторая нет
вторая строка пустая, первая нет
две пустые строки
одинаковые строки размер равен 0
*/

struct test_pack_strncmp_struct {
    char str1[20];
    char str2[20];
    int  size;
};
struct test_pack_strncmp_struct test_pack_strncmp[] = {
    { "abc",  "bcd",    5 },
    { "abc",  "bcd",    2 },
    { "abc",  "bcd",    0 },
    { "abcd", "afcf",   3 },
    { "a\0bc",  "abcd", 3 },
    { "abcd", "abc",    5 },
    { "",     "abc",    5 },
    { "",     "abc",    2 },
    { "",     "abc",    0 },
    { "abc",  "",       5 },
    { "abc",  "",       2 },
    { "abc",  "",       0 },
    { "",     "",       5 },
    { "",     "",       0 },
    { "abc",  "abc",    5 },
    { "abc",  "abc",    1 },
    { "abc",  "abc",    0 },
};

START_TEST(s21_strncmp_test) {
    int actual = s21_strncmp(test_pack_strncmp[_i].str1, test_pack_strncmp[_i].str2,
        test_pack_strncmp[_i].size);
    int expected = strncmp(test_pack_strncmp[_i].str1, test_pack_strncmp[_i].str2,
        test_pack_strncmp[_i].size);
    ck_assert_msg(actual == expected, MESSAGE_D, _i, expected, actual);
}

// s21_strcpy
/*
Тесты:
две строки с символами
копирование строки с символами к пустой строке
копирование пустой строки к строке с символами
копирование пустой строки к пустой строке
проверка на спец символы
*/

struct test_pack_strcpy_struct {
    char str1[20];
    char str2[20];
    char str3[20];
};
struct test_pack_strcpy_struct test_pack_strcpy[] = {
    { "Hello ", "Hello ", "Wor\0ld!" },
    { "",       "",       "World!" },
    { "Hello ", "Hello ", "" },
    { "",       "",       "" },
    { "\0",     "\0",     "\n" },
    { "\n",     "\n",     "\0" },
    { "\0",     "\0",     "\0" },
};

START_TEST(s21_strcpy_test) {
    char* actual = s21_strcpy(test_pack_strcpy[_i].str1, test_pack_strcpy[_i].str3);
    char* expected = strcpy(test_pack_strcpy[_i].str2, test_pack_strcpy[_i].str3);
    ck_assert_msg(strcmp(actual, expected) == 0, MESSAGE_S, _i, "", expected, actual);
}
END_TEST


// s21_strncpy
/*
Тесты:
копирование строки с символами к пустой строке, кол-во больше длинны строки
копирование строки с символами к пустой строке, кол-во меньше длинны строки
копирование пустой строки к строке с символами
копирование пустой строки к пустой строке
проверка на спец символы
копирование строки с символами к строке с символами, кол-во больше длинны строки
копирование строки с символами к строке с символами, кол-во меньше длинны строки
*/

struct test_pack_strncpy_struct {
    char str1[20];
    char str2[20];
    char str3[20];
    int  size;
};
struct test_pack_strncpy_struct test_pack_strncpy[] = {
    { "Hello ", "Hello ", "World!",   10 },
    { "Hello ", "Hello ", "World!",   3 },
    { "Hello ", "Hello ", "World!",   0 },
    { "",       "",       "World!",   10 },
    { "",       "",       "World!",   3 },
    { "",       "",       "World!",   0 },
    { "Hello ", "Hello ", "",         10 },
    { "Hello ", "Hello ", "",         3 },
    { "Hello ", "Hello ", "",         0 },
    { "",       "",       "",         2 },
    { "",       "",       "",         0 },
    { "\0",     "\0",     "\n",       3 },
    { "\0",     "\0",     "\n",       0 },
    { "\n",     "\n",     "\0",       3 },
    { "\n",     "\n",     "\0",       0 },
    { "\0",     "\0",     "\0",       3 },
    { "\0",     "\0",     "\0",       0 },
    { "",       "",       "Wor\0ld!", 10 },
};

START_TEST(s21_strncpy_test) {
    char* actual = s21_strncpy(test_pack_strncpy[_i].str1, test_pack_strncpy[_i].str3,
        test_pack_strncpy[_i].size);
    char* expected = strncpy(test_pack_strncpy[_i].str2, test_pack_strncpy[_i].str3,
        test_pack_strncpy[_i].size);
    ck_assert_msg(strcmp(actual, expected) == 0, MESSAGE_S, _i, "", expected, actual);
}
END_TEST


// s21_strcspn
/*
Тесты:
символы в строке присутствуют
символы в строке отсутствуют
поиск набора символов с начальным символом конца строки
проверка на спец символ переноса строки
поиск пустой строки в пустой строке
поиск символа конца строки в пустой строке
*/

struct test_pack_strcspn_struct {
    char str1[20];
    char str2[20];
};
struct test_pack_strcspn_struct test_pack_strcspn[] = {
    { "abcs7df", "mnbvcx7" },
    { "abcs7df", "mnbvx"},
    { "abcs7df", "\0abcs7df"},
    { "abcs7df", "\n"},
    { "",        ""},
    { "",        "\0"},
//
    { "abc",  "bcd" },
    { "abc",  "abcd"},
    { "abcd", "abc" },
    { "",     "abc" },
    { "abc",  ""},
    { "",     ""},
    { "abc",  "abc"},
};

START_TEST(s21_strcspn_test) {
    int actual = s21_strcspn(test_pack_strcspn[_i].str1, test_pack_strcspn[_i].str2);
    int expected = strcspn(test_pack_strcspn[_i].str1, test_pack_strcspn[_i].str2);
    ck_assert_msg(actual == expected, MESSAGE_D, _i + 1, expected, actual);
}


// s21_strerror

START_TEST(s21_strerror_test) {
    char* actual = s21_strerror(_i);
    char* expected = strerror(_i);
    ck_assert_str_eq(actual, expected);
}
END_TEST


// s21_strlen

char test_pack_strlen[][14] = {
    "Hello world\0",
    "Hello world\n\0",
    "a\n\0",
    " \n\0",
    " \0",
    "\n\0",
    "\0",
};

START_TEST(s21_strlen_test) {
    int actual = s21_strlen(test_pack_strlen[_i]);
    int expected = strlen(test_pack_strlen[_i]);
    ck_assert_msg(actual == expected, MESSAGE_D, _i + 1, expected, actual);
}
END_TEST


// s21_strpbrk
/*
Тесты:
поиск набора символов с символом \0 которые есть в строке с текстом
поиск набора символов в пустой строке
поиск символа \0 в строке с текстом
поиск символа \n в строке с текстом
проверка на спец символы
поиск набора символов которых нет в строке с текстом
*/

struct test_pack_strpbrk_struct {
    char str1[20];
    char str2[20];
    char str3[20];
};
struct test_pack_strpbrk_struct test_pack_strpbrk[] = {
    { "Hello World", "Hello World", "Wor\0ld!" },
    { "",            "",            "World!" },
    { "Hello ",      "Hello ",      "\0" },
    { "Hello \n",    "Hello \n",    "\n" },
    { "Hello ",      "Hello ",      "\n" },
    { "Hello ",      "Hello ",      "" },
    { "",            "",            "" },
    { "\0",          "\0",          "\n" },
    { "\n",          "\n",          "\0" },
    { "\0",          "\0",          "\0" },
    { "Hello World", "Hello World", "qwtyuip123456789" },
};

START_TEST(s21_strpbrk_test) {
    char* actual = s21_strpbrk(test_pack_strpbrk[_i].str1, test_pack_strpbrk[_i].str3);
    char* expected = strpbrk(test_pack_strpbrk[_i].str2, test_pack_strpbrk[_i].str3);
    // ck_assert_msg(strcmp(actual, expected) == 0, MESSAGE_S, _i + 1, "", expected, actual);
    ck_assert_pstr_eq(actual, expected);
}
END_TEST


// s21_strrchr
/*
Тесты:
символ в строке присутствует
символ в строке отсутствует
поиск символа конца строки
поиск символа переноса строки
поиск символа в пустой строке
поиск символа конца строки в пустой строке
*/

struct test_pack_strrchr_struct {
    char str1[20];
    char ch;
};
struct test_pack_strrchr_struct test_pack_strrchr[] = {
    { "abcs7df", '7' },
    { "abcs7df", 'e' },
    { "abcs7df", '\0' },
    { "abcs7df", '\n' },
    { "",        '5' },
    { "",        '\0' },
};

START_TEST(s21_strrchr_test) {
    char* actual = s21_strrchr(test_pack_strrchr[_i].str1, test_pack_strrchr[_i].ch);
    char* expected = strrchr(test_pack_strrchr[_i].str1, test_pack_strrchr[_i].ch);
    ck_assert_pstr_eq(actual, expected);
    // ck_assert_msg(strcmp(actual, expected) == 0, MESSAGE_S, _i + 1, "", expected, actual);
}
END_TEST


// s21_strspn
struct test_pack_strspn_struct {
    char str1[20];
    char str2[20];
};
struct test_pack_strspn_struct test_pack_strspn[] = {
    { "abcs7df", "mnbvcx7" },
    { "abcs7df", "mnbvx"},
    { "abcs7df", "\0abcs7df"},
    { "abcs7df", "\n"},
    { "",        ""},
    { "",        "\0"},
    { "abc",  "bcd" },
    { "abc",  "abcd"},
    { "abcd", "abc" },
    { "",     "abc" },
    { "abc",  ""},
    { "",     ""},
    { "abc",  "abc"},
};

START_TEST(s21_strspn_test) {
    int actual = s21_strspn(test_pack_strspn[_i].str1, test_pack_strspn[_i].str2);
    int expected = strspn(test_pack_strspn[_i].str1, test_pack_strspn[_i].str2);
    ck_assert_msg(actual == expected, MESSAGE_D, _i, expected, actual);
}


// s21_strstr
/*
Тесты:
поиск строки с символом \0 в строке с текстом
поиск строки с тексом в пустой строке
поиск строки \0 в строке с текстом
поиск строки \n в строке с текстом
проверка на спец символы
*/

struct test_pack_strstr_struct {
    char str1[20];
    char str2[20];
    char str3[20];
};
struct test_pack_strstr_struct test_pack_strstr[] = {
    { "Hello World", "Hello World", "World!" },
    { "Hello World", "Hello World", "Wor\0ld!" },
    { "",       "",       "World!" },
    { "Hello ", "Hello ", "\0" },
    { "Hello ", "Hello ", "\n" },
    { "Hello ", "Hello ", "" },
    { "",       "",       "" },
    { "\0",     "\0",     "\n" },
    { "\n",     "\n",     "\0" },
    { "\0",     "\0",     "\0" },
};

START_TEST(s21_strstr_test) {
    char* actual = s21_strstr(test_pack_strstr[_i].str1, test_pack_strstr[_i].str3);
    char* expected = strstr(test_pack_strstr[_i].str2, test_pack_strstr[_i].str3);
    ck_assert_pstr_eq(actual, expected);
}
END_TEST


// s21_strtok
/*
Тесты:
стандартная ситуация
в строке есть символ конца строки
строка пустая
символа разделителя нет в строке
проверка на спец символы
*/
struct test_pack_strtok_struct {
    char str1[20];
    char str2[20];
    char str3[20];
};
struct test_pack_strtok_struct test_pack_strtok[] = {
    { "Hello World",       "Hello World",       " ,." },
    { "H/el.lo\0 Wo/r,ld", "H/el.lo\0 Wo/r,ld", " \\,./0" },
    { "",                  "",                  " \\,./0" },
    { "H/el.lo\0 Wo/r,ld", "H/el.lo\0 Wo/r,ld", "x" },
    { "\0",                "\0",                "\n" },
    { "Hello World", "Hello World", "Wor\0ld!" },
    { "",       "",       "World!" },
    { "Hello ", "Hello ", "\0" },
    { "Hello ", "Hello ", "\n" },
    { "Hello ", "Hello ", "" },
    { "",       "",       "" },
    { "\n",     "\n",     "\0" },
    { "\0",     "\0",     "\0" },
};

START_TEST(s21_strtok_test) {
    char* actual = s21_strtok(test_pack_strtok[_i].str1, test_pack_strtok[_i].str3);
    char* expected = strtok(test_pack_strtok[_i].str2, test_pack_strtok[_i].str3);

    while (actual != NULL || expected != NULL) {
        ck_assert_pstr_eq(actual, expected);
        expected = strtok(NULL, test_pack_strtok[_i].str3);
        actual = s21_strtok(NULL, test_pack_strtok[_i].str3);
    }
    ck_assert_str_eq(test_pack_strtok[_i].str1, test_pack_strtok[_i].str2);
}
END_TEST


// доп. тесты

START_TEST(s21_bonus_test) {
    ck_assert_int_eq(s21_strncmp(NULL, NULL, 4), 0);
    ck_assert_int_eq(s21_strcmp(NULL, NULL), 0);
    ck_assert_pstr_eq(s21_strcat(NULL, NULL), NULL);
    ck_assert_pstr_eq(s21_strncat(NULL, NULL, 5), NULL);
    ck_assert_pstr_eq(s21_strcpy(NULL, NULL), NULL);
    char* buf = s21_insert("Hello", NULL, 2);
    ck_assert_pstr_eq(buf, "Hello");
    free(buf);
}
END_TEST


/*************************************************************************************************/
// s21_sprintf

// %c specifier

struct test_packc_struct {
    char fmt[50];
    char c;
};
struct test_packc_struct test_packc[] = {
    { "TEST %%\n", ' ' },
    { "TEST %20%\n", ' ' },
    { "TEST %-20%\n", ' ' },
    { "TEST %20.2%\n", ' ' },
    { "TEST %-20.2%\n", ' ' },
    { "TEST %c\n", 'A' },
    { "TEST %20c\n", 'A' },
    { "TEST %-20c\n", 'A' },
    { "TEST %20.2c\n", 'A' },
    { "TEST %-20.2c\n", 'A' },
};

START_TEST(s21_sprintf_test_packc) {
    s21_sprintf(actual_buf, test_packc[_i].fmt, test_packc[_i].c);
    sprintf(expected_buf, test_packc[_i].fmt, test_packc[_i].c);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i + 1, test_packc[_i].fmt,
        expected_buf, actual_buf);
}
END_TEST

START_TEST(s21_sprintf_test_c1) {
    char fmt[] = "AAA%cBBB%20cCCC";
    s21_sprintf(actual_buf, fmt, 'X', 'Y');
    sprintf(expected_buf, fmt, 'X', 'Y');
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i + 1, fmt, expected_buf, actual_buf);
}
END_TEST

// %s specifier

struct test_packs_struct {
    char fmt[50];
    char str[50];
};
struct test_packs_struct test_packs[] = {
    { "TEST %s\n", "A" },
    { "TEST %s\n", "IS OK!" },
    { "TEST %2s\n", "IS OK!" },
    { "TEST %20s\n", "IS OK!" },
    { "TEST %2.s\n", "IS OK!" },
    { "TEST %2.2s\n", "IS OK!" },
    { "TEST %20.2s\n", "IS OK!" },
    { "TEST %20.20sXXX\n", "IS OK!" },
    { "TEST %-20.20sXXX\n", "IS OK!" },
    { "TEST %-+ 20.20sXXX\n", "IS OK!" },
    { "TEST %-+ 20.-20sXXX\n", "IS OK!" },
};

START_TEST(s21_sprintf_test_packs) {
    s21_sprintf(actual_buf, test_packs[_i].fmt, test_packs[_i].str);
    sprintf(expected_buf, test_packs[_i].fmt, test_packs[_i].str);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, test_packs[_i].fmt,
        expected_buf, actual_buf);
}
END_TEST

START_TEST(s21_sprintf_test_s1) {
    char fmt[] = "AAA%sBBB%20.2sCCC%10.12sDDD";
    s21_sprintf(actual_buf, fmt, "XXXXX", "YYYYY", "ZZZZZ");
    sprintf(expected_buf, fmt, "XXXXX", "YYYYY", "ZZZZZ");
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, fmt, expected_buf, actual_buf);
}
END_TEST


// %d/i specifier

struct test_packd_struct {
    char fmt[50];
    long int num;
};
struct test_packd_struct test_packd[] = {
    { "%u\n", LONG_MAX },
    { "%hu\n", LONG_MAX },
    { "%lu\n", LONG_MAX },
    { "%u\n", LONG_MIN },
    { "%hu\n", LONG_MIN },
    { "%lu\n", LONG_MIN },
    { "%+1u\n", 100000000000 },
    { "%+hu\n", 100000000000 },
    { "%+lu\n", 100000000000 },
    { "% u\n", 100000000000 },
    { "% hu\n", 100000000000 },
    { "% lu\n", 100000000000 },

    { "%i\n", LONG_MAX },
    { "%hi\n", LONG_MAX },
    { "%li\n", LONG_MAX },
    { "%i\n", LONG_MIN },
    { "%hi\n", LONG_MIN },
    { "%li\n", LONG_MIN },

    { "%d", 111 },
    { "%i", 2111 },
    { "%+i", LONG_MIN+1 },
    { "%-i", LONG_MAX },
    { "% i", 2444 },
    { "%+ i", 2555 },
    { "%- i", 2666 },
    { "%+-i", 2777 },
    { "%+- i", 2888 },

    { "%2i",  2111 },
    { "%+2i", 2222 },
    { "%-2i", 2333 },
    { "% 2i", 2444 },
    { "%+ 2i", 2555 },
    { "%- 2i", 2666 },
    { "%+-2i", 2777 },
    { "%+- 2i", 2888 },

    { "%20i",  2111 },
    { "%+20i", 2222 },
    { "%-20i", 2333 },
    { "% 20i", 2444 },
    { "%+ 20i", 2555 },
    { "%- 20i", 2666 },
    { "%+-20i", 2777 },
    { "%+- 20i", 2888 },

    { "%+- 20.2i", 2888 },
    { "%+03i", 0 },
    { "E%+03i", 1 },
    { "%+020i", 2888 },
    { "%+020i", -2888 },

    { "%.5d", 777 },
    { "%0.0d", 0 },
    { "%0.0u", 0 },
};

START_TEST(s21_sprintf_test_packd) {
    s21_sprintf(actual_buf, test_packd[_i].fmt, test_packd[_i].num);
    sprintf(expected_buf, test_packd[_i].fmt, test_packd[_i].num);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, test_packd[_i].fmt,
        expected_buf, actual_buf);
}
END_TEST

START_TEST(s21_sprintf_test_d1) {
    char fmt[] = "%0*x";
    s21_sprintf(actual_buf, fmt, 8, 15);
    sprintf(expected_buf, fmt, 8, 15);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, fmt, expected_buf, actual_buf);
}
END_TEST


// %f specifier

struct test_packf_struct {
    char fmt[50];
    double num;
};
struct test_packf_struct test_packf[] = {
    { "%10f\n", 13./7. },
    { "%10.f\n", 13./7. },
    { "%1.1f\n", 13./7. },
    { "%15.1f\n", 13./7. },
    { "%15.15f\n", 13./7. },
    { "%1.1f\n", -13./7. },
    { "%15f\n", -13./7. },
    { "%15.f\n", -13./7. },
    { "%15.0f\n", -13./7. },
    { "%15.1f\n", -13./7. },
    { "%-15.15f\n", -13./7. },
    { "%-15.100f\n", -13./7. },
    { "%2.50f", 2./3. },
    { "%-15.5f\n", -13.399998 },
    { "%-15.5f\n", -19.999998 },
    { "%-15.5f\n", -99.999998 },
    { "%f", 1./0. },   // Infinity
    { "%f", -1./0. },  // −Infinity
    { "%f", INFINITY },
    { "%f", NAN },
    { "%f", 0./0. },   // Nan
    { "%f", 1 },
    { "%f.", 1 },
};

START_TEST(s21_sprintf_test_packf) {
    s21_sprintf(actual_buf, test_packf[_i].fmt, test_packf[_i].num);
    sprintf(expected_buf, test_packf[_i].fmt, test_packf[_i].num);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, test_packf[_i].fmt,
        expected_buf, actual_buf);
}
END_TEST

START_TEST(s21_sprintf_test_f1) {
    char fmt[] = "%0*.*f";
    s21_sprintf(actual_buf, fmt, 8, 4, 15./7.);
    sprintf(expected_buf, fmt, 8, 4, 15./7.);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, fmt, expected_buf, actual_buf);
}
END_TEST

// %e specifier

struct test_packe_struct {
    char fmt[50];
    double num;
};
struct test_packe_struct test_packe[] = {
    { "%e", 0 },
    { "%E\n", 13./7. },
    { "%e\n", 13./7. },
    { "%e\n", 7./13. },
    { "%e\n", 11111111111./13. },
    { "%10e\n", 13./7. },
    { "%10.e\n", 13./7. },
    { "%1.1e\n", 13./7. },
    { "%15.1e\n", 13./7. },
    { "%15.15e\n", 13./7. },
    { "%1.1e\n", -13./7. },
    { "%15e\n", -13./7. },
    { "%15.e\n", -13./7. },
    { "%15.0e\n", -13./7. },
    { "%15.1e\n", -13./7. },
    { "%-15.15e\n", -13./7. },
    { "%-15.100e\n", -13./7. },
    { "%2.50e", 2./3. },
    { "%-15.5e\n", -13.399998 },
    // { "%-15.5e\n", -19.999998 },
    // { "%-15.5e\n", -99.999998 },
    { "%e", -NAN },
};

START_TEST(s21_sprintf_test_packe) {
    s21_sprintf(actual_buf, test_packe[_i].fmt, test_packe[_i].num);
    sprintf(expected_buf, test_packe[_i].fmt, test_packe[_i].num);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, test_packe[_i].fmt,
        expected_buf, actual_buf);
}
END_TEST

START_TEST(s21_sprintf_test_e1) {
    char fmt[] = "AAA%0.2eBBB%10.0ECCC%-2.30eDDD\n";
    s21_sprintf(actual_buf, fmt, 8., 4., 15./7.);
    sprintf(expected_buf, fmt, 8., 4., 15./7.);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, fmt, expected_buf, actual_buf);
}
END_TEST


// %g specifier

struct test_packg_struct {
    char fmt[50];
    double num;
};
struct test_packg_struct test_packg[] = {
    { "%G", 0. },
    { "%G\n", 13./7. },
    { "%g\n", 13./7. },
    { "%g\n", 7./13. },
    { "%g\n", 11111111111./13. },
    { "%10g\n", 13./7. },
    { "%10.g\n", 13./7. },
    { "%1.1g\n", 13./7. },
    { "%15.1g\n", 13./7. },
    { "%15.15g\n", 13./7. },
    { "%1.1g\n", -13./7. },
    { "%15g\n", -13./7. },
    { "%15.g\n", -13./7. },
    { "%15.0g\n", -13./7. },
    { "%15.1g\n", -13./7. },
    { "%-15.15g\n", -13./7. },
    { "%-15.100g\n", -13./7. },
    { "%2.50g", 2./3. },
    { "%-15.5g\n", -13.399998 },
    { "%10.2G", 10.004 },
    { "%20.3G", 1000000 },
    { "%20.3G", 1100000 },
    // { "%-15.5e\n", -19.999998 },
    // { "%-15.5e\n", -99.999998 },
};

START_TEST(s21_sprintf_test_packg) {
    s21_sprintf(actual_buf, test_packg[_i].fmt, test_packg[_i].num);
    sprintf(expected_buf, test_packg[_i].fmt, test_packg[_i].num);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, test_packg[_i].fmt,
        expected_buf, actual_buf);
}
END_TEST

START_TEST(s21_sprintf_test_g1) {
    char fmt[] = "AAA%0.2GBBB%10.0gCCC%-2.30GDDD\n";
    s21_sprintf(actual_buf, fmt, 8., 4., 15./7.);
    sprintf(expected_buf, fmt, 8., 4., 15./7.);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, fmt, expected_buf, actual_buf);
}
END_TEST


// %x specifier

struct test_packx_struct {
    char fmt[50];
    unsigned long num;
};
struct test_packx_struct test_packx[] = {
    { "%x\n", 256 },
    { "%X\n", 234567890 },
    { "%x\n",   ULONG_MAX },
    { "%hx\n",  ULONG_MAX },
    { "%lx\n",  ULONG_MAX },
    { "%x\n",   0 },
    { "%hx\n",  0 },
    { "%lx\n",  0 },
    { "%+1x\n", 100000000000 },
    { "%+hx\n", 100000000000 },
    { "%+lx\n", 100000000000 },
    { "% x\n",  100000000000 },
    { "% hx\n", 100000000000 },
    { "% lx\n", 100000000000 },
    { "%#x\n", 234567890 },
    { "%#X\n", 234567890 },
    { "%0.0x", 0 },
    // { "%-+#2.30x\n", 0x33333333},
};

START_TEST(s21_sprintf_test_packx) {
    s21_sprintf(actual_buf, test_packx[_i].fmt, test_packx[_i].num);
    sprintf(expected_buf, test_packx[_i].fmt, test_packx[_i].num);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, test_packx[_i].fmt,
        expected_buf, actual_buf);
}
END_TEST

// START_TEST(s21_sprintf_test_x1) {
//     char fmt[] = "AAA%0.2hxBBB%10.0XCCC%-2.30lxDDD\n";
//     s21_sprintf(actual_buf, fmt, 11111, 22222222, 33333333);
//     sprintf(expected_buf, fmt, 11111, 22222222, 33333333);
//     ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, fmt, expected_buf, actual_buf);
// }
// END_TEST


// %o specifier

struct test_packo_struct {
    char fmt[50];
    unsigned long num;
};
struct test_packo_struct test_packo[] = {
    { "%o\n", 256 },
    { "%o\n", 234567890 },
    { "%o\n",   ULONG_MAX },
    { "%ho\n",  ULONG_MAX },
    { "%lo\n",  ULONG_MAX },
    { "%o\n",   0 },
    { "%ho\n",  0 },
    { "%lo\n",  0 },
    { "%+1o\n", 100000000000 },
    { "%+ho\n", 100000000000 },
    { "%+lo\n", 100000000000 },
    { "% o\n",  100000000000 },
    { "% ho\n", 100000000000 },
    { "% lo\n", 100000000000 },
    { "%#o\n", 256 },
    { "%#020o\n", 256 },
    { "%0.0o", 0 },
    { "%#0.0o", 0 },
};

START_TEST(s21_sprintf_test_packo) {
    s21_sprintf(actual_buf, test_packo[_i].fmt, test_packo[_i].num);
    sprintf(expected_buf, test_packo[_i].fmt, test_packo[_i].num);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, test_packo[_i].fmt,
        expected_buf, actual_buf);
}
END_TEST

START_TEST(s21_sprintf_test_o1) {
    char fmt[] = "AAA%0.2hoBBB%10.0oCCC%-2.30loDDD\n";
    s21_sprintf(actual_buf, fmt, 11111, 22222222, 33333333);
    sprintf(expected_buf, fmt, 11111, 22222222, 33333333);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, fmt, expected_buf, actual_buf);
}
END_TEST


// %n specifier

// char test_packn[][50] = {
//     "1%n",
//     "1234567890%n",
// };
// int buf1 = -1;
// int buf2 = -1;

// START_TEST(s21_sprintf_test_packn) {

//     // s21_sprintf(actual_buf, test_packn[_i].fmt, &buf1);
//     sprintf(expected_buf, test_packn[_i], &buf2);
//     ck_assert_int_eq(buf1, buf2);
// }
// END_TEST


// %p specifier

struct test_packp_struct {
    char fmt[50];
    int* ptr;
};
struct test_packp_struct test_packp[] = {
    { "%p\n",   (int*)0x12345699 },
    { "%20p\n", (int*)0x12345678 },
    { "%0.0p",  (int*)0 },
};

START_TEST(s21_sprintf_test_packp) {
    s21_sprintf(actual_buf, test_packp[_i].fmt, test_packp[_i].ptr);
    sprintf(expected_buf, test_packp[_i].fmt, test_packp[_i].ptr);
    ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, test_packp[_i].fmt,
        expected_buf, actual_buf);
}
END_TEST

// START_TEST(s21_sprintf_test_p1) {
//     char fmt[] = "AAA%0.2pBBB%10.0pCCC%-2.30pDDD\n";
//     s21_sprintf(actual_buf, fmt, 0x11111111, 0x22222222, 0x33333333);
//     sprintf(expected_buf, fmt, 0x11111111, 0x22222222, 0x33333333);
//     ck_assert_msg(strcmp(actual_buf, expected_buf) == 0, MESSAGE_S, _i, fmt, expected_buf, actual_buf);
// }
// END_TEST


// return test

START_TEST(s21_sprintf_test_return) {
    int actual_ret = s21_sprintf(actual_buf, "% 20.20f", -13./7.);
    int expected_ret = sprintf(expected_buf, "% 20.20f", -13./7.);
    ck_assert_int_eq(actual_ret, expected_ret);
}
END_TEST


/*************************************************************************************************/
// part5: to_upper, to_lower, insert, trim

// s21_to_upper test

struct test_pack_s21_to_upper_struct {
    char str1[20];
    char str2[20];
};
struct test_pack_s21_to_upper_struct test_pack_s21_to_upper[] = {
    { "Hello World", "HELLO WORLD"},
    { "Hello\0 World", "HELLO"},
    { "",       ""},
    { "Hel\nlo ", "HEL\nLO "},
    { "Hello_12324/..~§ ", "HELLO_12324/..~§ "},
    { "1233289475/.,\\][{}", "1233289475/.,\\][{}"},
    { "\0",     "\0"},
    { "\n",     "\n"},
};

START_TEST(s21_to_upper_test) {
    char* actual = s21_to_upper(test_pack_s21_to_upper[_i].str1);
    char* expected = test_pack_s21_to_upper[_i].str2;
    ck_assert_pstr_eq(actual, expected);
    free(actual);
    ck_assert_pstr_eq(s21_to_upper(NULL), NULL);
}
END_TEST


// s21_to_lower test

struct test_pack_s21_to_lower_struct {
    char str1[20];
    char str2[20];
};
struct test_pack_s21_to_lower_struct test_pack_s21_to_lower[] = {
    { "Hello World", "hello world"},
    { "Hello\0 World", "hello"},
    { "",       ""},
    { "heL\nLo ", "hel\nlo "},
    { "Hello_12324/..~§ ", "hello_12324/..~§ "},
    { "1233289475/.,\\][{}", "1233289475/.,\\][{}"},
    { "\0Hello World",     "\0"},
    { "\n",     "\n"},
};

START_TEST(s21_to_lower_test) {
    char* actual = s21_to_lower(test_pack_s21_to_lower[_i].str1);
    char* expected = test_pack_s21_to_lower[_i].str2;
    ck_assert_pstr_eq(actual, expected);
    free(actual);
    ck_assert_pstr_eq(s21_to_lower(NULL), NULL);
}
END_TEST


// s21_insert

// verter tests
// char *res_str = s21_insert(src_str1, "= ", 8);
// res_str = s21_insert(src_str1, "abobich", 20);
// res_str = s21_insert(NULL, "asdf", 3);
// res_str = s21_insert(src_str1, "", 1);

struct test_pack_insert_struct {
    char str1[50];
    char str2[10];
    int  pos;
    char result[50];
};
struct test_pack_insert_struct test_pack_insert[] = {
    { "xxxxxxx 123", "= ", 8, "xxxxxxx = 123" },
    { "xxxxxxx 123", "abobich", 20, "xxxxxxx 123" },
    { "xxxxxxx 123", "", 1 , "xxxxxxx 123"},
};

START_TEST(s21_insert_test) {
    char* buf = s21_insert(test_pack_insert[_i].str1, test_pack_insert[_i].str2,
        test_pack_insert[_i].pos);
    ck_assert_pstr_eq(buf, test_pack_insert[_i].result);
    free(buf);
}
END_TEST

START_TEST(s21_insert_test2) {
    char* buf = s21_insert((char*)s21_NULL, "asdf", 3);
    ck_assert_pstr_eq(buf, s21_NULL);
    free(buf);
}
END_TEST


// s21_trim

START_TEST(s21_trim_test) {
    char* buf = s21_trim("123mama456", "123456");
    ck_assert_pstr_eq(buf, "mama");
    free(buf);
}
END_TEST


/*************************************************************************************************/
// main

int main() {
    Suite *s = suite_create("s21_strings test");    // устанавливает название всего набора тестов
    SRunner *sr = srunner_create(s);                // подключает набор к утилите для запуска тестов
    srunner_set_fork_status(sr, CK_NOFORK);         // настраивает возможность отключить запуск тестов
                                                    // в несколько потоков
    // functions
    TCase *tc_funcs = tcase_create("functions case");    // тесты могут быть разбиты на группы
    suite_add_tcase(s, tc_funcs);                   // подключает группу тестов к набору
    tcase_set_timeout(tc_funcs, 10);                // устанавливает максимальное время исполнения

    // s21_memchr
    int test_pack_memchr_size = sizeof(test_pack_memchr) / sizeof(test_pack_memchr[0]);
    tcase_add_loop_test(tc_funcs, s21_memchr_test, 0, test_pack_memchr_size);
    tcase_add_test(tc_funcs, s21_sprintf_test_c1);

    // s21_memcmp
    int test_pack_memcmp_size = sizeof(test_pack_memcmp) / sizeof(test_pack_memcmp[0]);
    tcase_add_loop_test(tc_funcs, s21_memcmp_test, 0, test_pack_memcmp_size);
    // tcase_add_test(tc_funcs, s21_memcmp_test2);

    // s21_memcpy
    int test_pack_memcpy_size = sizeof(test_pack_memcpy) / sizeof(test_pack_memcpy[0]);
    tcase_add_loop_test(tc_funcs, s21_memcpy_test, 0, test_pack_memcpy_size);

    // s21_memmove
    int test_pack_memmove_size = sizeof(test_pack_memmove) / sizeof(test_pack_memmove[0]);
    tcase_add_loop_test(tc_funcs, s21_memmove_test_1, 0, test_pack_memmove_size);
    tcase_add_loop_test(tc_funcs, s21_memmove_test_2, 0, test_pack_memmove_size);

    int test_pack_memmove_size3 = sizeof(test_pack_memmove3) / sizeof(test_pack_memmove3[0]);
    tcase_add_loop_test(tc_funcs, s21_memmove_test_3, 0, test_pack_memmove_size3);

    // s21_memset
    int test_pack_memset_size = sizeof(test_pack_memset) / sizeof(test_pack_memset[0]);
    tcase_add_loop_test(tc_funcs, s21_memset_test, 0, test_pack_memset_size);

    // s21_strcat
    int test_pack_strcat_size = sizeof(test_pack_strcat) / sizeof(test_pack_strcat[0]);
    tcase_add_loop_test(tc_funcs, s21_strcat_test, 0, test_pack_strcat_size);

    // s21_strncat
    int test_pack_strncat_size = sizeof(test_pack_strncat) / sizeof(test_pack_strncat[0]);
    tcase_add_loop_test(tc_funcs, s21_strncat_test, 0, test_pack_strncat_size);

    // s21_strchr
    int test_pack_strchr_size = sizeof(test_pack_strchr) / sizeof(test_pack_strchr[0]);
    tcase_add_loop_test(tc_funcs, s21_strchr_test, 0, test_pack_strchr_size);

    // s21_strcmp
    int test_pack_strcmp_size = sizeof(test_pack_strcmp) / sizeof(test_pack_strcmp[0]);
    tcase_add_loop_test(tc_funcs, s21_strcmp_test, 0, test_pack_strcmp_size);

    // s21_strncmp
    int test_pack_strncmp_size = sizeof(test_pack_strncmp) / sizeof(test_pack_strncmp[0]);
    tcase_add_loop_test(tc_funcs, s21_strncmp_test, 0, test_pack_strncmp_size);

    // s21_strcpy
    int test_pack_strcpy_size = sizeof(test_pack_strcpy) / sizeof(test_pack_strcpy[0]);
    tcase_add_loop_test(tc_funcs, s21_strcpy_test, 0, test_pack_strcpy_size);

    // s21_strncpy
    int test_pack_strncpy_size = sizeof(test_pack_strncpy) / sizeof(test_pack_strncpy[0]);
    tcase_add_loop_test(tc_funcs, s21_strncpy_test, 0, test_pack_strncpy_size);

    // s21_strcspn
    int test_pack_strcspn_size = sizeof(test_pack_strcspn) / sizeof(test_pack_strcspn[0]);
    tcase_add_loop_test(tc_funcs, s21_strcspn_test, 0, test_pack_strcspn_size);

    // s21_strerror
    tcase_add_loop_test(tc_funcs, s21_strerror_test, -5, ERROR_LIMIT + 5);

    // s21_strlen
    int test_pack_strlen_size = sizeof(test_pack_strlen) / sizeof(test_pack_strlen[0]);
    tcase_add_loop_test(tc_funcs, s21_strlen_test, 0, test_pack_strlen_size);

    // s21_strpbrk
    int test_pack_strpbrk_size = sizeof(test_pack_strpbrk) / sizeof(test_pack_strpbrk[0]);
    tcase_add_loop_test(tc_funcs, s21_strpbrk_test, 0, test_pack_strpbrk_size);

    // s21_strrchr
    int test_pack_strrchr_size = sizeof(test_pack_strrchr) / sizeof(test_pack_strrchr[0]);
    tcase_add_loop_test(tc_funcs, s21_strrchr_test, 0, test_pack_strrchr_size);

    // s21_strspn
    int test_pack_strspn_size = sizeof(test_pack_strspn) / sizeof(test_pack_strspn[0]);
    tcase_add_loop_test(tc_funcs, s21_strspn_test, 0, test_pack_strspn_size);

    // s21_strstr
    int test_pack_strstr_size = sizeof(test_pack_strstr) / sizeof(test_pack_strstr[0]);
    tcase_add_loop_test(tc_funcs, s21_strstr_test, 0, test_pack_strstr_size);

    // s21_strtok
    int test_pack_strtok_size = sizeof(test_pack_strtok) / sizeof(test_pack_strtok[0]);
    tcase_add_loop_test(tc_funcs, s21_strtok_test, 0, test_pack_strtok_size);

    tcase_add_test(tc_funcs, s21_bonus_test);

    // s21_sprintf
    TCase *tc_sprintf = tcase_create("sprintf case");
    suite_add_tcase(s, tc_sprintf);
    tcase_set_timeout(tc_sprintf, 10);

    // %c/% specifier
    int packc_size = sizeof(test_packc) / sizeof(test_packc[0]);
    tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packc, 0, packc_size);

    // %s specifier
    int packs_size = sizeof(test_packs) / sizeof(test_packs[0]);
    tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packs, 0, packs_size);
    tcase_add_test(tc_sprintf, s21_sprintf_test_s1);

    // %d/i specifier
    int packd_size = sizeof(test_packd) / sizeof(test_packd[0]);
    tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packd, 0, packd_size);
    tcase_add_test(tc_sprintf, s21_sprintf_test_d1);

    // %f specifier
    int packf_size = sizeof(test_packf) / sizeof(test_packf[0]);
    tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packf, 0, packf_size);
    tcase_add_test(tc_sprintf, s21_sprintf_test_f1);

    // %e specifier
    int packe_size = sizeof(test_packe) / sizeof(test_packe[0]);
    tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packe, 0, packe_size);
    tcase_add_test(tc_sprintf, s21_sprintf_test_e1);

    // %g specifier
    int packg_size = sizeof(test_packg) / sizeof(test_packg[0]);
    tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packg, 0, packg_size);
    tcase_add_test(tc_sprintf, s21_sprintf_test_g1);

    // %x specifier
    int packx_size = sizeof(test_packx) / sizeof(test_packx[0]);
    tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packx, 0, packx_size);
    // tcase_add_test(tc_sprintf, s21_sprintf_test_x1);

    // %o specifier
    int packo_size = sizeof(test_packo) / sizeof(test_packo[0]);
    tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packo, 0, packo_size);
    tcase_add_test(tc_sprintf, s21_sprintf_test_o1);

    // %n specifier
    // int packn_size = sizeof(test_packn) / sizeof(test_packn[0]);
    // tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packn, 0, packn_size);

    // %p specifier
    int packp_size = sizeof(test_packp) / sizeof(test_packp[0]);
    tcase_add_loop_test(tc_sprintf, s21_sprintf_test_packp, 0, packp_size);
    // tcase_add_test(tc_sprintf, s21_sprintf_test_p1);

    // return test
    tcase_add_test(tc_sprintf, s21_sprintf_test_return);

    //*******

    // s21_to_upper
    int test_pack_s21_to_upper_size = sizeof(test_pack_s21_to_upper) / sizeof(test_pack_s21_to_upper[0]);
    tcase_add_loop_test(tc_funcs, s21_to_upper_test, 0, test_pack_s21_to_upper_size);

    // s21_to_lower
    int test_pack_s21_to_lower_size = sizeof(test_pack_s21_to_lower) / sizeof(test_pack_s21_to_lower[0]);
    tcase_add_loop_test(tc_funcs, s21_to_lower_test, 0, test_pack_s21_to_lower_size);

    // s21_insert
    int test_pack_insert_size = sizeof(test_pack_insert) / sizeof(test_pack_insert[0]);
    tcase_add_loop_test(tc_funcs, s21_insert_test, 0, test_pack_insert_size);
    tcase_add_test(tc_funcs, s21_insert_test2);

    // s21_trim
    tcase_add_test(tc_funcs, s21_trim_test);

    srunner_run_all(sr, CK_ENV);                    // запускает тесты
    srunner_free(sr);                               // освобождает ресурсы
    return 0;                                       // this is the way
}
