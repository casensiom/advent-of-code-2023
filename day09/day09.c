#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

SInteger
solve_recursive(SIntegerArray list)
{
    bool full_zeros = true;
    SIntegerArray l = AC_ARRAY_CREATE(SInteger, list.count - 1);
    for (size_t i = 1; i < list.count; i++)
    {
        SInteger a = (list.items[i] - list.items[i - 1]);
        AC_ARRAY_INSERT(l, a);
        full_zeros &= (a == 0);
    }

    SInteger last = list.items[list.count - 1];
    SInteger ret = 0;
    if (!full_zeros)
    {
        ret = solve_recursive(l);
    }

    AC_ARRAY_DESTROY(l);
    return ret + last;
}

SInteger
solve_recursive2(SIntegerArray list)
{
    bool full_zeros = true;
    SIntegerArray l = AC_ARRAY_CREATE(SInteger, list.count - 1);
    for (size_t i = 1; i < list.count; i++)
    {
        SInteger a = (list.items[i] - list.items[i - 1]);
        AC_ARRAY_INSERT(l, a);
        full_zeros &= (a == 0);
    }

    SInteger first = list.items[0];
    SInteger ret = 0;
    if (!full_zeros)
    {
        ret = solve_recursive2(l);
    }

    AC_ARRAY_DESTROY(l);
    return first - ret;
}

size_t
part1(StringArray lines)
{
    size_t total = 0;
    for (size_t i = 0; i < lines.count; i++)
    {
        // parse
        size_t temp;
        SIntegerArray list = parse_signed_integer_list(lines.items[i]);
        ssize_t r = solve_recursive(list);
        total += r;
        AC_ARRAY_DESTROY(list);
    }

    return total;
}

size_t
part2(StringArray lines)
{
    size_t total = 0;
    for (size_t i = 0; i < lines.count; i++)
    {
        // parse
        size_t temp;
        SIntegerArray list = parse_signed_integer_list(lines.items[i]);
        ssize_t r = solve_recursive2(list);
        total += r;
        AC_ARRAY_DESTROY(list);
    }

    return total;
}

int main(int argc, char **argv)
{
    CharArray content = read_file_content("assets/input01.txt");
    StringArray lines = string_split(content, '\n');

    size_t p1 = part1(lines);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(lines);
    printf(" Part 2 total:  %lu\n", p2);

    AC_ARRAY_DESTROY(lines);
    AC_ARRAY_DESTROY(content);
    return 0;
}
