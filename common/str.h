#ifndef _STR_H_
#define _STR_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "array.h"

typedef char Char;
AC_ARRAY_DEFINE(Char);

typedef CharArray String;
AC_ARRAY_DEFINE(String);

#define STRING_FMT "%.*s"
#define STRING_ARG(str) (int)(str).count, (str).items
#define STRING_NPOS (size_t)(-1)

#define string_count(data, item) string_count_from(data, item, 0)
size_t string_count_from(String data, char item, size_t offset);

#define string_split(data, delim) string_split_ex(data, delim, false)
StringArray string_split_ex(String data, char delim, bool keep_empty); /// requires destroy result

String string_join_with_char(StringArray data, char join);     /// requires destroy result
String string_join_with_string(StringArray data, String join); /// requires destroy result
String string_join(StringArray data);                          /// requires destroy result
String string_concat(String data, String other);               /// requires destroy result

String string_create(const char *str);
String string_create_with_len(const char *str, size_t len);

String string_clone(const char *str);                      /// requires destroy result
String string_clone_with_len(const char *str, size_t len); /// requires destroy result
String string_clone_with_string(String data);              /// requires destroy result

size_t string_find(String data, String other);
size_t string_find_from(String data, String other, size_t offset);
bool string_equals(String data, String other);
bool string_equals_case(String data, String other);
bool string_starts_with(String data, String prefix);
bool string_ends_with(String data, String suffix);

String string_ltrim(String data);
String string_rtrim(String data);
String string_trim(String data);

size_t string_remove(String *data, char chr);

#endif // _STR_H_

#ifdef STRING_IMPLEMENTATION

size_t
string_count_from(String data, char item, size_t offset)
{
    size_t count = 0;
    for (size_t i = offset; i < data.count; i++)
    {
        if (data.items[i] == item)
        {
            ++count;
        }
    }
    return count;
}

StringArray
string_split_ex(String data, char delim, bool keep_empty)
{
    size_t count = string_count(data, delim);
    StringArray list = AC_ARRAY_CREATE(String, count + 1);

    size_t pos = 0;
    size_t start = 0;
    while (pos < data.count)
    {
        if (data.items[pos] == delim)
        {
            size_t len = pos - start;
            if (len > 0 || keep_empty)
            {
                list.items[list.count++] = AC_ARRAY_SET(Char, data.items + start, len);
            }
            start = pos + 1;
        }
        ++pos;
    }
    size_t len = pos - start;
    if (len > 0 || keep_empty)
    {
        list.items[list.count++] = AC_ARRAY_SET(Char, data.items + start, len);
    }
    return list;
}

String
string_join_with_char(StringArray data, char join)
{
    String tmp = string_create_with_len(&join, 1);
    return string_join_with_string(data, tmp);
}

String
string_join_with_string(StringArray data, String join)
{
    String ret = {0};
    if (data.count == 0)
    {
        return ret;
    }

    size_t size = 0;
    for (size_t i = 0; i < data.count; i++)
    {
        size += data.items[i].count;
    }
    size += (data.count - 1) * join.count;

    ret = AC_ARRAY_CREATE(Char, size);
    for (size_t i = 0; i < data.count; i++)
    {
        for (size_t j = 0; j < data.items[i].count; j++)
        {
            AC_ARRAY_PUSH(ret, data.items[i].items[j]);
        }
        if (i < data.count - 1)
        {
            for (size_t j = 0; j < join.count; j++)
            {
                AC_ARRAY_PUSH(ret, join.items[j]);
            }
        }
    }
    return ret;
}

String
string_join(StringArray data)
{
    String tmp = string_create_with_len(NULL, 0);
    return string_join_with_string(data, tmp);
}

String
string_concat(String data, String other)
{
    String ret = {0};
    if (data.count == 0 && other.count == 0)
    {
        return ret;
    }
    else if (data.count == 0)
    {
        return other;
    }
    else if (other.count == 0)
    {
        return data;
    }

    size_t size = data.count + other.count + 1;
    ret = AC_ARRAY_CREATE(Char, size);
    strncpy(ret.items, data.items, data.count);
    strncpy(ret.items + data.count, other.items, other.count);
    ret.items[ret.capacity - 1] = '\0';
    ret.count = ret.capacity - 1;

    return ret;
}

String
string_create(const char *str)
{
    size_t len = strlen(str);
    return AC_ARRAY_SET(Char, str, len);
}

String
string_create_with_len(const char *str, size_t len)
{
    return AC_ARRAY_SET(Char, str, len);
}

String
string_clone(const char *str)
{
    size_t len = strlen(str);
    return string_clone_with_len(str, len);
}

String
string_clone_with_len(const char *str, size_t len)
{
    String ret = AC_ARRAY_CREATE(Char, len);
    strncpy(ret.items, str, len);
    ret.count = len;
    return ret;
}

String
string_clone_with_string(String data)
{
    return string_clone_with_len(data.items, data.count);
}

size_t
string_find(String data, String other)
{
    return string_find_from(data, other, 0);
}

size_t
string_find_from(String data, String other, size_t offset)
{
    size_t ret = STRING_NPOS;
    if (other.count > data.count)
    {
        return ret;
    }
    for (size_t i = offset; i < (data.count - (other.count - 1)); i++)
    {
        if (string_starts_with(string_create_with_len(data.items + i, data.count - i), other))
        {
            ret = i;
            break;
        }
    }
    return ret;
}

bool string_equals(String data, String other)
{
    if (data.count != other.count)
    {
        return false;
    }
    for (size_t i = 0; i < data.count; i++)
    {
        if (tolower(data.items[i]) != tolower(other.items[i]))
        {
            return false;
        }
    }

    return true;
}

bool string_equals_case(String data, String other)
{
    if (data.count != other.count)
    {
        return false;
    }
    return (strncmp(data.items, other.items, other.count) == 0);
}

bool string_starts_with(String data, String prefix)
{
    if (data.count < prefix.count)
    {
        return false;
    }
    return string_equals_case(string_create_with_len(data.items, prefix.count), prefix);
}
bool string_ends_with(String data, String suffix)
{
    if (data.count < suffix.count)
    {
        return false;
    }
    return string_equals_case(string_create_with_len(data.items + data.count - suffix.count, suffix.count), suffix);
}

String
string_ltrim(String data)
{
    while (data.count > 0 && isspace(data.items[0]))
    {
        data.items++;
        data.count--;
    }
    return data;
}

String
string_rtrim(String data)
{
    while (data.count > 0 && isspace(data.items[data.count - 1]))
    {
        data.count--;
    }
    return data;
}

String
string_trim(String data)
{
    return string_ltrim(string_rtrim(data));
}

size_t
string_remove(String *data, char chr)
{
    size_t ret = 0;
    size_t i = 0;
    size_t j = 0;
    while (j < data->count)
    {
        if (data->items[j] == chr)
        {
            while (j < data->count && data->items[j] == chr)
            {
                j++;
                ret++;
            }
        }
        else
        {
            data->items[i] = data->items[j];
            i++;
            j++;
        }
    }
    data->count -= ret;
    data->items[data->count] = '\0';
    return ret;
}

#endif