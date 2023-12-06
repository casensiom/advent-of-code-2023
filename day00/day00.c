#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

size_t
part1(StringArray lines)
{
    return 0;
}

size_t
part2(StringArray lines)
{
    return 0;
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
