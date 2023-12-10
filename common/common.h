#ifndef _COMMON_H_
#define _COMMON_H_

#include "array.h"
#include "str.h"

#ifndef max
 #define max(a,b) \
   ({ typeof (a) _a = (a); \
       typeof (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif
#ifndef min
 #define min(a,b) \
   ({ typeof (a) _a = (a); \
       typeof (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif

String read_file_content(const char *file);

String parser_consume_chars(String *data, size_t num_chars);
String parser_consume_literal(String *data);
String parser_consume_space(String *data);
size_t parser_consume_int(String *data, String *out);
float parser_consume_float(String *data, String *out);

typedef size_t Integer;
AC_ARRAY_DEFINE(Integer);

typedef ssize_t SInteger;
AC_ARRAY_DEFINE(SInteger);

IntegerArray parse_integer_list(String data);
SIntegerArray parse_signed_integer_list(String data);

#endif