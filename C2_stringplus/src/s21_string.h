#ifndef SRC_S21_STRING_H_
#define SRC_S21_STRING_H_


#define s21_NULL (void*) 0
typedef unsigned long s21_size_t;

#define TRUE  1
#define FALSE 0
typedef unsigned short bool;

#define _MAX_I64TOSTR_BASE10_COUNT 65


void *s21_memchr(const void *str, int c, s21_size_t n);
// Searches for the first occurrence of the character c (an unsigned char) in the first n bytes of the string
// pointed to, by the argument str.

int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
// Compares the first n bytes of str1 and str2.

void *s21_memcpy(void *dest, const void *src, s21_size_t n);
// Copies n characters from src to dest.

void *s21_memmove(void *dest, const void *src, s21_size_t n);
// Another function to copy n characters from str2 to str1.

void *s21_memset(void *str, int c, s21_size_t n);
// Copies the character c (an unsigned char) to the first n characters of the string pointed to, by the
// argument str.

char *s21_strcat(char *dest, const char *src);
// Appends the string pointed to, by src to the end of the string pointed to by dest.

char *s21_strncat(char *dest, const char *src, s21_size_t n);
// Appends the string pointed to, by src to the end of the string pointed to, by dest up to n characters long.

char *s21_strchr(const char *str, int c);
// Searches for the first occurrence of the character c (an unsigned char) in the string pointed to, by the
// argument str.

int s21_strcmp(const char *str1, const char *str2);
// Compares the string pointed to, by str1 to the string pointed to by str2.

int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
// Compares at most the first n bytes of str1 and str2.

char *s21_strcpy(char *dest, const char *src);
// Copies the string pointed to, by src to dest.

char *s21_strncpy(char *dest, const char *src, s21_size_t n);
// Copies up to n characters from the string pointed to, by src to dest.

s21_size_t s21_strcspn(const char *str1, const char *str2);
// Calculates the length of the initial segment of str1 which consists entirely of characters not in str2.

#ifdef __APPLE__
    #define ERROR_LIMIT 107
#else
    #define ERROR_LIMIT 134
#endif

char *s21_strerror(int errnum);
// Searches an internal array for the error number errnum and returns a pointer to an error message string.
// You need to declare macros containing arrays of error messages for mac and linux operating systems.
// Error descriptions are available in the original library. Checking the current OS is carried out using
// directives.

s21_size_t s21_strlen(const char *str);
// Computes the length of the string str up to but not including the terminating null character.

char *s21_strpbrk(const char *str1, const char *str2);
// Finds the first character in the string str1 that matches any character specified in str2.

char *s21_strrchr(const char *str, int c);
// Searches for the last occurrence of the character c (an unsigned char) in the string pointed to by the
// argument str.

s21_size_t s21_strspn(const char *str1, const char *str2);
// Calculates the length of the initial segment of str1 which consists entirely of characters in str2.

char *s21_strstr(const char *haystack, const char *needle);
// Finds the first occurrence of the entire string needle (not including the terminating null character)
// which appears in the string haystack.

char *s21_strtok(char *str, const char *delim);
// Breaks string str into a series of tokens separated by delim.

int s21_sprintf(char *str, const char *format, ...);
// Partial implementation of the sprintf function
// Specifiers: c, d, i, f, s, u, %, e, E, x, X, o, n, p
// Flags: -, +, (space), 0, #
// Width description: (number), *
// Precision description: .(number), *
// Length description: h, l, L

int sscanf(const char *str, const char *format, ...);

void *s21_to_upper(const char *str);
// Returns a copy of string (str) converted to uppercase. In case of any error, return NULL

void *s21_to_lower(const char *str);
// Returns a copy of string (str) converted to lowercase. In case of any error, return NULL

s21_size_t s21_strrspn(const char *str1, const char *str2);

void *s21_insert(const char *src, const char *str, size_t start_index);
// Returns a new string in which a specified string (str) is inserted at a specified index position
// (start_index) in the given string (src). In case of any error, return NULL

void *s21_trim(const char *src, const char *trim_chars);
// Returns a new string in which all leading and trailing occurrences of a set of specified characters
// (trim_chars) from the given string (src) are removed. In case of any error, return NULL

#endif  // SRC_S21_STRING_H_
