#include "common.h"

#define STRING_IMPLEMENTATION
#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

String
read_file_content(const char *file)
{
    String content = {0};
    FILE *fp = fopen(file, "r");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        int count = ftell(fp);

        fseek(fp, 0, SEEK_SET);
        content = AC_ARRAY_CREATE(Char, count);
        while (1)
        {
            char c = fgetc(fp);
            if (feof(fp) || content.count >= content.capacity)
            {
                break;
            }
            content.items[content.count++] = c;
        }

        fclose(fp);
    }
    else
    {
        printf("[ERROR] the file '%s' doesn't exists.\n", file);
    }

    if (content.count == 0)
    {
        printf("[ERROR] the file '%s' have no content.\n", file);
    }
    return content;
}

String parser_consume_chars(String *data, size_t num_chars)
{
    if (num_chars > data->count)
    {
        num_chars = data->count;
    }

    String ret = string_create_with_len(data->items, num_chars);
    data->items += num_chars;
    data->count -= num_chars;
    return ret;
}

String parser_consume_literal(String *data)
{
    size_t start = 0;
    size_t pos = 0;
    while (pos < data->count && isalnum(data->items[pos]))
    {
        ++pos;
    }

    size_t len = pos - start;
    String ret = string_create_with_len(data->items, len);

    data->items += len;
    data->count -= len;
    return ret;
}

String parser_consume_space(String *data)
{
    size_t start = 0;
    size_t pos = 0;
    while (pos < data->count && isspace(data->items[pos]))
    {
        ++pos;
    }

    size_t len = pos - start;
    String ret = string_create_with_len(data->items, len);

    data->items += len;
    data->count -= len;
    return ret;
}

size_t parser_consume_int(String *data, String *out)
{
    char *ptr = data->items;
    size_t ret = strtoll(ptr, &ptr, 10);
    size_t len = ptr - data->items;
    if (out != NULL)
    {
        *out = string_create_with_len(data->items, len);
    }
    data->items = ptr;
    data->count -= len;
    return ret;
}

float parser_consume_float(String *data, String *out)
{
    char *ptr = data->items;
    float ret = strtof(ptr, &ptr);
    size_t len = ptr - data->items;
    if (out != NULL)
    {
        *out = string_create_with_len(data->items, len);
    }
    data->items = ptr;
    data->count -= len;
    return ret;
}

IntegerArray
parse_integer_list(String data)
{
    return parse_integer_list_with_delim(data, ' ');
}

IntegerArray
parse_integer_list_with_delim(String data, char delim)
{
    StringArray list = string_split(data, delim);
    IntegerArray ret = AC_ARRAY_CREATE(Integer, list.count);

    for (size_t i = 0; i < list.count; i++)
    {
        AC_ARRAY_INSERT(ret, parser_consume_int(&(list.items[i]), NULL));
    }

    AC_ARRAY_DESTROY(list);
    return ret;
}

SIntegerArray
parse_signed_integer_list(String data)
{
    IntegerArray list = parse_integer_list(data);
    return (SIntegerArray){.items = (SInteger *)(list.items), .count = (list.count), .capacity = list.capacity};
}
