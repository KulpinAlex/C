#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include "s21_string.h"


// s21_memchr

void *s21_memchr(const void *str, int c, size_t n) {
    void* dest = s21_NULL;
    char* src = (char*)str;
    for (size_t length = 0; length < n && str; length++, src++) {
        if (*src == (char)c) {
            dest = src;
            break;
        }
    }
    return dest;
}


// s21_memcmp

int s21_memcmp(const void *str1, const void *str2, size_t n) {
    int result = 0, pos1 = 0, pos2 = 0;
    for (; n-- && str1 && str2; pos1++, pos2++) {
        unsigned char dest1 = ((unsigned char *) str1)[pos1];
        unsigned char dest2 = ((unsigned char *) str2)[pos2];
        if (dest1 != dest2) {
            // result = (dest1 > dest2) ? 1 : -1;
            result = dest1 - dest2;
            break;
        }
    }
    return result;
}


// s21_memcpy

void *s21_memcpy(void *dest, const void *src, size_t n) {
    if (dest != s21_NULL && src != s21_NULL) {
        if (dest <= src) {
            for (size_t i = 0; i < n; i++) {
                ((char*)dest)[i] = ((char*)src)[i];
            }
        } else {
            for (size_t i = n; i > 0; i--) {
                ((char*)dest)[i - 1] = ((char*)src)[i - 1];
            }
        }
    }
    return dest;
}


// s21_memmove

void *s21_memmove(void *dest, const void *src, size_t n) {
    return s21_memcpy(dest, src, n);
}


// s21_memset

void *s21_memset(void *str, int c, s21_size_t n) {
    for (size_t i = 0; i < n && str; i++) {
        ((char*)str)[i] = (char)c;
    }
    return str;
}


// s21_strcat

char* s21_strcat(char* dst, const char* src) {
    char* nulled = "";
    if (src == s21_NULL)
        src = nulled;
    if (dst != s21_NULL) {
        s21_size_t dst_len = s21_strlen(dst);
        s21_size_t src_len = s21_strlen(src);
        for (s21_size_t i = 0; i <= src_len; i++)
            dst[dst_len + i] = src[i];
    }
    return dst;
}


// s21_strncat

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
    char* nulled = "";
    if (src == s21_NULL)
        src = nulled;
    if (dest != s21_NULL) {
        s21_size_t dest_len = s21_strlen(dest);
        s21_size_t src_len = (s21_strlen(src) > n) ? n : s21_strlen(src);
        for (s21_size_t i = 0; i < src_len; i++)
            dest[dest_len + i] = src[i];
    }
    return dest;
}


// s21_strchr

char* s21_strchr(const char* str, int c) {
    char* result = s21_NULL;
    if (str != s21_NULL) {
        int pos = 0;
        for (; (*(str + pos)) && (*(str + pos) != c); pos++) {}
        result = (*(str + pos) == c) ? (char*) str + pos : s21_NULL;
    }
    return result;
}


// s21_strcmp

int s21_strcmp(const char* str1, const char* str2) {
    char* nulled = "";
    if (str1 == s21_NULL)
        str1 = nulled;
    if (str2 == s21_NULL)
        str2 = nulled;
    for (; *str1 && *str1 == *str2; str1++, str2++) {}
    return *str1 - *str2;
    // return (*str1 == *str2) ? 0 : ((*str1 > *str2) ? 1 : -1);
}


// s21_strncmp

int s21_strncmp(const char* str1, const char* str2, s21_size_t num) {
    char* nulled = "";
    if (str1 == s21_NULL)
        str1 = nulled;
    if (str2 == s21_NULL)
        str2 = nulled;
    s21_size_t i = 0;
    for (; *str1 && *str2 && *str1 == *str2 && i < num; str1++, str2++, i++) {}
    return i < num ? *str1 - *str2 : 0;
}


// s21_strcpy

char* s21_strcpy(char* dst, const char* src) {
    char* nulled = "";
    if (src == s21_NULL)
        src = nulled;
    if (dst != s21_NULL) {
        unsigned long len = s21_strlen(src);
        for (unsigned long i = 0; i <= len; i++)
            dst[i] = src[i];
    }
    return dst;
}


// s21_strncpy

char* s21_strncpy(char* dst, const char* src, s21_size_t num) {
    if (src != s21_NULL && dst != s21_NULL) {
        s21_size_t i = 0;
        for (; i < num && src[i] != 0; i++) {
            dst[i] = src[i];
        }
        if (i < num) {
            dst[i] = 0;
        }
    }
    return dst;
}


// s21_strcspn

s21_size_t s21_strcspn(const char *str1, const char *str2) {
    s21_size_t result = s21_strlen(str1);
    for (s21_size_t i = 0; i < s21_strlen(str1); ++i) {
        if (s21_strchr((char*) str2, str1[i]) != s21_NULL) {
            result = i;
            break;
        }
    }
    return result;
}


// s21_strerror

#ifdef __APPLE__
    const char* err_mes = "Unknown error: ";
    char ER_MAC_OS[][80] = {
        "Undefined error: 0",
        "Operation not permitted",
        "No such file or directory",
        "No such process",
        "Interrupted system call",
        "Input/output error",
        "Device not configured",
        "Argument list too long",
        "Exec format error",
        "Bad file descriptor",
        "No child processes",
        "Resource deadlock avoided",
        "Cannot allocate memory",
        "Permission denied",
        "Bad address",
        "Block device required",
        "Resource busy",
        "File exists",
        "Cross-device link",
        "Operation not supported by device",
        "Not a directory",
        "Is a directory",
        "Invalid argument",
        "Too many open files in system",
        "Too many open files",
        "Inappropriate ioctl for device",
        "Text file busy",
        "File too large",
        "No space left on device",
        "Illegal seek",
        "Read-only file system",
        "Too many links",
        "Broken pipe",
        "Numerical argument out of domain",
        "Result too large",
        "Resource temporarily unavailable",
        "Operation now in progress",
        "Operation already in progress",
        "Socket operation on non-socket",
        "Destination address required",
        "Message too long",
        "Protocol wrong type for socket",
        "Protocol not available",
        "Protocol not supported",
        "Socket type not supported",
        "Operation not supported",
        "Protocol family not supported",
        "Address family not supported by protocol family",
        "Address already in use",
        "Can't assign requested address",
        "Network is down",
        "Network is unreachable",
        "Network dropped connection on reset",
        "Software caused connection abort",
        "Connection reset by peer",
        "No buffer space available",
        "Socket is already connected",
        "Socket is not connected",
        "Can't send after socket shutdown",
        "Too many references: can't splice",
        "Operation timed out",
        "Connection refused",
        "Too many levels of symbolic links",
        "File name too long",
        "Host is down",
        "No route to host",
        "Directory not empty",
        "Too many processes",
        "Too many users",
        "Disc quota exceeded",
        "Stale NFS file handle",
        "Too many levels of remote in path",
        "RPC struct is bad",
        "RPC version wrong",
        "RPC prog. not avail",
        "Program version wrong",
        "Bad procedure for program",
        "No locks available",
        "Function not implemented",
        "Inappropriate file type or format",
        "Authentication error",
        "Need authenticator",
        "Device power is off",
        "Device error",
        "Value too large to be stored in data type",
        "Bad executable (or shared library)",
        "Bad CPU type in executable",
        "Shared library version mismatch",
        "Malformed Mach-o file",
        "Operation canceled",
        "Identifier removed",
        "No message of desired type",
        "Illegal byte sequence",
        "Attribute not found",
        "Bad message",
        "EMULTIHOP (Reserved)",
        "No message available on STREAM",
        "ENOLINK (Reserved)",
        "No STREAM resources",
        "Not a STREAM",
        "Protocol error",
        "STREAM ioctl timeout",
        "Operation not supported on socket",
        "Policy not found",
        "State not recoverable",
        "Previous owner died",
        "Interface output queue is full",
    };
#else
    const char* err_mes = "Unknown error ";
    char ERR_GNU_LINUX[][80] = {
        "Success",
        "Operation not permitted",
        "No such file or directory",
        "No such process",
        "Interrupted system call",
        "Input/output error",
        "No such device or address",
        "Argument list too long",
        "Exec format error",
        "Bad file descriptor",
        "No child processes",
        "Resource temporarily unavailable",
        "Cannot allocate memory",
        "Permission denied",
        "Bad address",
        "Block device required",
        "Device or resource busy",
        "File exists",
        "Invalid cross-device link",
        "No such device",
        "Not a directory",
        "Is a directory",
        "Invalid argument",
        "Too many open files in system",
        "Too many open files",
        "Inappropriate ioctl for device",
        "Text file busy",
        "File too large",
        "No space left on device",
        "Illegal seek",
        "Read-only file system",
        "Too many links",
        "Broken pipe",
        "Numerical argument out of domain",
        "Numerical result out of range",
        "Resource deadlock avoided",
        "File name too long",
        "No locks available",
        "Function not implemented",
        "Directory not empty",
        "Too many levels of symbolic links",
        "Unknown error 41",
        "No message of desired type",
        "Identifier removed",
        "Channel number out of range",
        "Level 2 not synchronized",
        "Level 3 halted",
        "Level 3 reset",
        "Link number out of range",
        "Protocol driver not attached",
        "No CSI structure available",
        "Level 2 halted",
        "Invalid exchange",
        "Invalid request descriptor",
        "Exchange full",
        "No anode",
        "Invalid request code",
        "Invalid slot",
        "Unknown error 58",
        "Bad font file format",
        "Device not a stream",
        "No data available",
        "Timer expired",
        "Out of streams resources",
        "Machine is not on the network",
        "Package not installed",
        "Object is remote",
        "Link has been severed",
        "Advertise error",
        "Srmount error",
        "Communication error on send",
        "Protocol error",
        "Multihop attempted",
        "RFS specific error",
        "Bad message",
        "Value too large for defined data type",
        "Name not unique on network",
        "File descriptor in bad state",
        "Remote address changed",
        "Can not access a needed shared library",
        "Accessing a corrupted shared library",
        ".lib section in a.out corrupted",
        "Attempting to link in too many shared libraries",
        "Cannot exec a shared library directly",
        "Invalid or incomplete multibyte or wide character",
        "Interrupted system call should be restarted",
        "Streams pipe error",
        "Too many users",
        "Socket operation on non-socket",
        "Destination address required",
        "Message too long",
        "Protocol wrong type for socket",
        "Protocol not available",
        "Protocol not supported",
        "Socket type not supported",
        "Operation not supported",
        "Protocol family not supported",
        "Address family not supported by protocol",
        "Address already in use",
        "Cannot assign requested address",
        "Network is down",
        "Network is unreachable",
        "Network dropped connection on reset",
        "Software caused connection abort",
        "Connection reset by peer",
        "No buffer space available",
        "Transport endpoint is already connected",
        "Transport endpoint is not connected",
        "Cannot send after transport endpoint shutdown",
        "Too many references: cannot splice",
        "Connection timed out",
        "Connection refused",
        "Host is down",
        "No route to host",
        "Operation already in progress",
        "Operation now in progress",
        "Stale file handle",
        "Structure needs cleaning",
        "Not a XENIX named type file",
        "No XENIX semaphores available",
        "Is a named type file",
        "Remote I/O error",
        "Disk quota exceeded",
        "No medium found",
        "Wrong medium type",
        "Operation canceled",
        "Required key not available",
        "Key has expired",
        "Key has been revoked",
        "Key was rejected by service",
        "Owner died",
        "State not recoverable",
        "Operation not possible due to RF-kill",
        "Memory page has hardware error",
    };
#endif


// перевод числа в строку
void int_to_char(char* result, int number) {
    int flag = number > 0 ? 1 : -1;
    number *= flag;
    int i = 0;
    while (number > 9) {
        result[i++] = number % 10 + '0';
        number /= 10;
    }
    result[i] = number + '0';
    if (flag == -1) {
        i += 1;
        result[i] = '-';
    }

    for (int b = 0; b <= i/2; ++b) {
        int temp = result[b];
        result[b] = result[i - b];
        result[i - b] = temp;
    }
    result[i+1] = '\0';
}


char* s21_strerror(int errnum) {
    char* result = "";
    static char temp[1000] = {0};
    static char sborka[1000] = {0};
    temp[0] = '\0';
    sborka[0] = '\0';
    int_to_char(temp, errnum);

#ifdef __APPLE__
    if (errnum >= 0 && errnum < ERROR_LIMIT) {
        result = ER_MAC_OS[errnum];
    }
#else
    if (errnum >= 0 && errnum < ERROR_LIMIT) {
        result = ERR_GNU_LINUX[errnum];
    }
#endif
    if (errnum < 0 || errnum >= ERROR_LIMIT) {
     result = s21_strcat(s21_strcat(sborka, err_mes), temp);
    }

    return result;
}


// s21_strlen

s21_size_t s21_strlen(const char* str) {
    s21_size_t length = 0;
    if (str != s21_NULL) {
        for (; *(str + length); length++) {}
    }
    return length;
}


// s21_strpbrk

char *s21_strpbrk(const char *str1, const char *str2) {
    char* result = s21_NULL;
    s21_size_t num = s21_strcspn(str1, str2);
    if (num < s21_strlen(str1)) {
        result = (char*) str1 + num;
    }
    return result;
}


// s21_strrchr

char* s21_strrchr(const char* str, int symbol) {
    char* result = s21_NULL;
    if (str != s21_NULL) {
        s21_size_t pos = s21_strlen(str) - 1;
        for (; (*(str + pos)) && (*(str + pos) != symbol); pos--) {}
        result = (*(str + pos) == symbol) ? (char*)str + pos : s21_NULL;
    }
    return result;
}


// s21_strspn

s21_size_t s21_strspn(const char *str1, const char *str2) {
    s21_size_t result = 0;
    for (s21_size_t i = 0; i <= s21_strlen(str1); ++i) {
        result = i;
        if (s21_strchr(str2, str1[i]) == s21_NULL) {
            break;
        }
    }
    return result;
}


// s21_strstr

char *s21_strstr(const char *haystack, const char *needle) {
    char* result = s21_NULL;
    if (s21_strlen(needle) == 0) {
        result = (char*)haystack;
    } else if ((haystack != s21_NULL) && (needle != s21_NULL)) {
        while (*haystack && (result == s21_NULL)) {
            char* start = (char*)haystack;
            char* find = (char*)needle;
            do {
                haystack++;
                find++;
            } while (*haystack && *find && *haystack == *find);
            if (!*find)
                result = start;
        }
    }
    if (s21_strlen(needle) == 1 && needle[0] != haystack[0]) {
        result = s21_NULL;
    }
    return result;
}


// s21_strtok

char* s21_strtok(char* str, const char* delim) {
    static char* last;
    char* result;
    if (str) {
        last = str;
        while (s21_strchr(delim, *last) && *last) {
            *last = 0;
            last++;
        }
    }
    if (*last) {
        result = last;
        while (s21_strchr(delim, *last) == s21_NULL && *last) {
            last++;
        }
        while (s21_strchr(delim, *last) && *last) {
            *last = 0;
            last++;
        }
    } else {
        result = s21_NULL;
    }
    return result;
}


/*************************************************************************************************/
// s21_sprintf

struct format_out {
    bool flag_leftalign;    // выравнивание слева
    bool flag_sign;         // флаг для '+'
    bool flag_space;        // флаг для (space)
    bool flag_sharp;        // флаг для '#'
    bool flag_zerofill;     // заполнение нулями

    bool add_len;           // дополнительный операнд длины из va_arg
    int len;                // длина числа
    bool add_prec;          // дополнительный операнд длины для дробной части
    int precision;          // длина дробной части

    bool is_char;           // модификатор 'hh'
    bool is_short;          // модификатор 'h'
    bool is_long;           // модификатор 'l'
    bool is_longdouble;     // модификатор 'L'

    char specificator;
};


struct format_out parser(const char* substr, int len, va_list argptr) {
    // Parse format string to format_out struct (for sprintf)
    // %[flags][width][.precision][length]specifier
    struct format_out result;
    result.specificator   = substr[len - 1];
    // flags
    result.flag_leftalign = FALSE;
    result.flag_sign      = FALSE;
    result.flag_space     = FALSE;
    result.flag_sharp     = FALSE;
    result.flag_zerofill  = FALSE;
    // width & precision
    result.add_len        = FALSE;
    result.len            = 0;
    result.add_prec       = FALSE;
    result.precision      = (s21_strchr("feEgG", result.specificator) != s21_NULL) ? 6 : -1;
    // length
    result.is_char        = FALSE;
    result.is_short       = FALSE;
    result.is_long        = FALSE;
    result.is_longdouble  = FALSE;

    int i = 0;

    // flags
    for (; s21_strchr("-+ #0", substr[i]) != NULL; i++) {
        if (substr[i] == '-') {
            result.flag_leftalign = TRUE;
        }
        if (substr[i] == '+' && s21_strchr("difFeEgGaA", result.specificator)) {
            result.flag_sign = TRUE;
        }
        if (substr[i] == ' ' && s21_strchr("difFeEgGaA", result.specificator)) {
            result.flag_space = TRUE;
        }
        if (substr[i] == '#') {
            result.flag_sharp = TRUE;
        }
        if (substr[i] == '0') {
            result.flag_zerofill = TRUE;
        }
    }

    // width OR ignore for scanf
    if (substr[i] == '*') {
        i++;
        result.add_len = TRUE;
        result.len = va_arg(argptr, int);
    } else {
        char* p_end;
        result.len = strtol(substr + i, &p_end, 10);
        i += p_end - (substr + i);
    }

    // precision
    if (substr[i] == '.') {
        i++;
        if (substr[i] == '*') {
            result.add_prec = TRUE;
            result.precision = va_arg(argptr, int);
            i++;
        } else {
            char* p_end;
            result.precision = strtol(substr + i, &p_end, 10);
            if (result.precision < 0) {
                result.precision = 0;
            }
            i += p_end - (substr + i);
        }
    }

    // length
    switch (substr[i]) {
        case 'h':
            if (substr[i + 1] == 'h') {
                result.is_char = TRUE;
            } else {
                result.is_short = TRUE;
            }
            break;
        case 'l':
            result.is_long  = TRUE;
            break;
        case 'L':
            result.is_longdouble = TRUE;
    }

    return result;
}


char* s21_ltoa(unsigned long int val, int base) {
    // UL to string
    static char buf[_MAX_I64TOSTR_BASE10_COUNT] = {0};
    int i = _MAX_I64TOSTR_BASE10_COUNT - 1;
    if (val == 0) {
        buf[0] = '0';
        return buf;
    }
    s21_memset(buf, '\0', _MAX_I64TOSTR_BASE10_COUNT);
    for (; val && i ; --i, val /= base) {
        buf[i] = "0123456789abcdef"[val % base];
    }
    return &buf[i+1];
}


char* s21_ftoa(long double val, struct format_out fmt) {
    char* result;

    // .float to string
    long op_integer = (fmt.precision == 0) ? roundl(val) : (long)val;
    val = val - op_integer;
    char* buffer_integer = s21_ltoa(op_integer, 10);

    // 3 = null-term + '.' + old-9-to-10
    result = calloc(s21_strlen(buffer_integer) + fmt.precision + 3, 1);
    s21_strcpy(result, buffer_integer);

    int pos = s21_strlen(buffer_integer);
    if (fmt.precision > 0 || fmt.flag_sharp) {
        result[pos++] = '.';
    }

    for (int i = 0; i < fmt.precision; i++, pos++) {
        val = val * 10;
        result[pos] = "0123456789abcdef"[(int)val];
        val = val - (int)val;
    }
    // round
    if (val * 10 >= 5) {
        int j = 0;
        while ((result[pos - j - 1] == '9') || (result[pos - j - 1] == '.')) {
            if (result[pos - j - 1] == '.') {
                j++;
            }
            result[pos - j - 1] = '0';
            j++;
        }
        if (pos - j - 1 >= 0) {
            result[pos - j - 1]++;
        } else {
            s21_memmove(result + 1, result, j);
            result[0] = '1';
        }
    }

    return result;
}


struct exp_num {
    long double mantissa;
    int exponent;
};

struct exp_num get_exponent(long double val) {
    struct exp_num result = { val, 0 };
    while (result.mantissa < 1 && result.mantissa > 0) {
        result.exponent--;
        result.mantissa *= 10;
    }
    while (result.mantissa >= 10) {
        result.exponent++;
        result.mantissa /= 10;
    }
    return result;
}


char* s21_trimrzeros(char* buffer) {
    int buf_pos = s21_strlen(buffer) - 1;
    int i = 0;
    for (; buffer[buf_pos - i] == '0'; i++) {
        buffer[buf_pos - i] = '\0';
    }
    if (buffer[buf_pos - i] == '.') {
        buffer[buf_pos - i] = '\0';
    }
    return buffer;
}


char* s21_etoa(long double val, struct format_out fmt, bool truncate_zeros) {
    struct exp_num val_exp = get_exponent(val);
    char* buffer = s21_ftoa(val_exp.mantissa, fmt);
    if (s21_strncmp(buffer, "10", 2) == 0) {
        buffer[s21_strlen(buffer) - 1] = 0;
        buffer[1] = buffer[2];
        buffer[2] = '0';
        val_exp.exponent++;
    }
    if (truncate_zeros) {
        buffer = s21_trimrzeros(buffer);
    }
    char* e_buffer = calloc(s21_strlen(buffer) + 5, sizeof(char));  // buffer + "e+00\0"
    s21_strncpy(e_buffer, buffer, s21_strlen(buffer));
    char* e_format = (fmt.specificator == 'e') ? "e%+03i" : "E%+03i";
    s21_sprintf(e_buffer + s21_strlen(buffer), e_format, val_exp.exponent);
    free(buffer);
    return e_buffer;
}


char* string_to_result(char* str, struct format_out fmt) {
    int str_len = s21_strlen(str);
    if (fmt.precision > -1 && fmt.precision < str_len) {
        str_len = fmt.precision;
    }
    int result_len = (fmt.len > str_len) ? fmt.len : str_len;
    char* result = calloc(result_len + 1, sizeof(char));
    s21_memset(result, ' ', result_len);
    if (fmt.flag_leftalign) {
        s21_strncpy(result, str, str_len);
    } else {
        s21_strncpy(&result[result_len - str_len], str, str_len);
    }
    return result;
}


char* unsigned_to_result(char* buffer, bool minus, struct format_out fmt) {
    bool sign = minus || fmt.flag_sign || fmt.flag_space;
    int result_len = s21_strlen(buffer) + sign;
    int buffer_len = result_len;
    if (result_len < fmt.len) {
        buffer_len = fmt.len;
    }

    char* result = calloc(buffer_len + 1, sizeof(char));
    char fill_ch = (fmt.flag_zerofill) ? '0' : ' ';
    s21_memset(result, fill_ch, buffer_len);

    int res_pos = 0;

    if (!fmt.flag_leftalign && !fmt.flag_zerofill) {
        res_pos += buffer_len - result_len;
    }

    if (sign) {
        result[res_pos] = ' ';
        if (fmt.flag_sign) {
            result[res_pos] = '+';
        }
        if (minus) {
            result[res_pos] = '-';
        }
        res_pos++;
    }

    if (fmt.flag_zerofill) {
        res_pos += buffer_len - result_len;
    }

    s21_memcpy(result + res_pos, buffer, result_len - sign);
    return result;
}


char* proceed_string_spec(struct format_out fmt, va_list argptr) {
    char buf[] = "%";
    char* in_str = &buf[0];
    if (fmt.specificator == 'c') {
        in_str[0] = (char)va_arg(argptr, int);
    }
    if (fmt.specificator == 's') {
        in_str = va_arg(argptr, char*);
    }
    return string_to_result(in_str, fmt);
}


char* proceed_integer_spec(struct format_out fmt, va_list argptr) {
    unsigned long op;
    bool minus = FALSE;

    if (s21_strchr("uxXop", fmt.specificator) != s21_NULL) {
        if (fmt.is_long || fmt.specificator == 'p') {
            op = va_arg(argptr, unsigned long);
        } else if (fmt.is_char) {
            op = (unsigned char) va_arg(argptr, unsigned int);
        } else if (fmt.is_short) {
            op = (unsigned short) va_arg(argptr, unsigned int);
        } else {
            op = va_arg(argptr, unsigned int);
        }
    }

    if (fmt.specificator == 'd' || fmt.specificator == 'i') {
        long op_longint;
        if (fmt.is_long) {
            op_longint = va_arg(argptr, long);
        } else if (fmt.is_char) {
            op_longint = (char) va_arg(argptr, int);
        } else if (fmt.is_short) {
            op_longint = (short) va_arg(argptr, int);
        } else {
            op_longint = (long)va_arg(argptr, int);
        }

        minus = (op_longint < 0);
        op = (minus) ? (unsigned long)(-op_longint) : (unsigned long)op_longint;
    }

    char buf[] = "";
    char* buffer = &buf[0];

    if (s21_strchr("diu", fmt.specificator) != s21_NULL) {
        buffer = (op == 0 && fmt.precision == 0) ? buffer : s21_ltoa(op, 10);
    }

    if (s21_strchr("xXp", fmt.specificator) != s21_NULL) {
        buffer = (op == 0 && fmt.precision == 0) ? buffer : s21_ltoa(op, 16);
        if (fmt.specificator == 'p' || (fmt.flag_sharp && op != 0)) {
            buffer -= 2;
            s21_memcpy(buffer, "0x", 2);
        }
        if (fmt.specificator == 'X') {
            char* result = s21_to_upper(buffer);
            s21_strcpy(buffer, result);
            free(result);
        }
    }

    if (fmt.specificator == 'o') {
        buffer = (op == 0 && fmt.precision == 0) ? buffer : s21_ltoa(op, 8);
        if (fmt.flag_sharp) {
            buffer--;
            buffer[0] = '0';
        }
    }

    int zerofill = fmt.precision - s21_strlen(buffer);
    if (zerofill > 0) {
        buffer -= zerofill;
        s21_memset(buffer, '0', zerofill);
    }

    return unsigned_to_result(buffer, minus, fmt);
}


char* proceed_float_spec(struct format_out fmt, va_list argptr) {
    long double arg;
    char* result;
    char* buffer = "";

    if (fmt.is_longdouble) {
        arg = va_arg(argptr, long double);
    } else {
        arg = (long double)va_arg(argptr, double);
    }

    bool minus = (arg < 0);
    if (minus) {
        arg = -arg;
    }
    if (arg == INFINITY || arg != arg) {
        buffer = calloc(4, sizeof(char));
        s21_strcpy(buffer, (arg == INFINITY) ? "inf" : "nan");
    } else {
        if (fmt.specificator == 'g' || fmt.specificator == 'G') {
            // Правило:
            // P — это точность (или 6, если точность не указана, или 1, если точность равна нулю),
            // X — десятичный показатель степени, необходимый для записи в стиле E/e, тогда:
            // если P > X ≥ −4 , преобразование выполняется со стилем f или F и точностью P − 1 − X
            // в противном случае выполняется преобразование со стилем e или E и точностью P − 1

            struct exp_num arg_exp = get_exponent(arg);
            int p = (fmt.precision == 0) ? 1 : fmt.precision;
            if (arg_exp.exponent >= -4 && arg_exp.exponent < p) {
                fmt.precision = p - 1 - arg_exp.exponent;
                buffer = s21_ftoa(arg, fmt);
            } else {
                fmt.precision--;
                fmt.specificator = (fmt.specificator == 'g') ? 'e' : 'E';
                buffer = s21_etoa(arg, fmt, TRUE);
            }
            if (fmt.precision > 0 && !fmt.flag_sharp) {
                buffer = s21_trimrzeros(buffer);
            }
        } else if (fmt.specificator == 'f') {
            buffer = s21_ftoa(arg, fmt);
        } else if (fmt.specificator == 'e' || fmt.specificator == 'E') {
            buffer = s21_etoa(arg, fmt, FALSE);
        }
    }

    result = unsigned_to_result(buffer, minus, fmt);
    free(buffer);
    return result;
}


int s21_sprintf(char *str, const char *format, ...) {
// Partial implementation of the sprintf function
// Specifiers: c, d, i, f, s, u, %, e, E, g, G, x, X, o, n, p
// Flags: -, +, (space), 0, #
// Width description: (number), *
// Precision description: .(number), *
// Length description: h, l, L
    va_list argptr;
    va_start(argptr, format);
    int pos = 0;
    int i = 0;
    for (; format[i]; i++) {
        if (format[i] == '%') {
            // get start and finish symbol of format,  parse format str
            s21_size_t fmt_len = s21_strcspn(format + i + 1, "cdifsueEgGxXonp%");
            struct format_out fmt = parser(format + i + 1, fmt_len + 1, argptr);
            i = i + fmt_len + 1;

            char* result = NULL;

            if (s21_strchr("cs%", fmt.specificator) != s21_NULL) {
                result = proceed_string_spec(fmt, argptr);
            }

            if (s21_strchr("diuxXop", fmt.specificator) != s21_NULL) {
                result = proceed_integer_spec(fmt, argptr);
            }

            if (s21_strchr("feEgG", fmt.specificator) != s21_NULL) {
                result = proceed_float_spec(fmt, argptr);
            }

            if (fmt.specificator == 'n') {
                int* write = va_arg(argptr, int*);
                *write = pos;
            }

            // copy result to destination
            if (result != NULL) {
                s21_strcpy(str + pos, result);
                pos += s21_strlen(result);
                free(result);
            }

        } else {
            // copy all another symbols to destination
            str[pos++] = format[i];
        }
    }
    str[pos] = format[i];
    va_end(argptr);
    return s21_strlen(str);
}


// s21_to_upper

void *s21_to_upper(const char *str) {
    char* result = s21_NULL;
    if (str != s21_NULL) {
        result = calloc(s21_strlen(str) + 1, sizeof(char));
        for (size_t i = 0; i < s21_strlen(str); i++) {
            result[i] = str[i];
            if (result[i] >= 'a' && result[i] <= 'z') {
                result[i] -= 0x20;
            }
        }
    }
    return result;
}


// s21_to_lower

void *s21_to_lower(const char *str) {
    char* result = s21_NULL;
    if (str != s21_NULL) {
        result = calloc(s21_strlen(str) + 1, sizeof(char));
        for (size_t i = 0; i < s21_strlen(str); i++) {
            result[i] = str[i];
            if (result[i] >= 'A' && result[i] <= 'Z') {
                result[i] += 0x20;
            }
        }
    }
    return result;
}


// s21_insert

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
    char* result = s21_NULL;
    if (str == s21_NULL) {
        str = "";
    }
    if (src) {
        int result_len = s21_strlen(str) + s21_strlen(src);
        result = calloc(result_len + 1, sizeof(char));
        if (start_index <= s21_strlen(src)) {
            s21_memcpy(result, src, start_index);
            s21_memcpy(result + start_index, str, s21_strlen(str));
            s21_strcpy(result + start_index + s21_strlen(str), src + start_index);
        } else {
            s21_strcpy(result, src);
        }
    }
    return result;
}


// s21_trim

s21_size_t s21_strrspn(const char *str1, const char *str2) {
    s21_size_t result = 0;
    for (int i = s21_strlen(str1) - 1; i >= 0 && s21_strchr(str2, str1[i]); i--, result++) {}
    return result;
}

void *s21_trim(const char *src, const char *trim_chars) {
    char* result = s21_NULL;
    if (src) {
        if (trim_chars == s21_NULL) {
            trim_chars = "";
        }
        s21_size_t trim_start_len = s21_strspn(src, trim_chars);
        s21_size_t trim_end_len = s21_strrspn(src + trim_start_len, trim_chars);
        result = calloc(s21_strlen(src) + 1, sizeof(char));
        s21_memcpy(result, src + trim_start_len, s21_strlen(src) - trim_start_len - trim_end_len);
    }
    return result;
}
