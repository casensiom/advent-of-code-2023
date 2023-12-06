#include "common.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int
get_number(const char *str, size_t pos, int rev) {
    char * numbers[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    size_t count     = 9;
    for(size_t i = 0; i < count; i++) {
        int         len   = strlen(numbers[i]);
        const char *start = str + pos;
        if(rev) {
            start -= (len - 1);
        }
        if(memcmp(start, numbers[i], len) == 0) {
            return (i + 1);
        }
    }
    return 0;
}

int
check_digit(const char *str, size_t pos, int rev) {
    if(isdigit(str[pos])) {
        return 1;
    }
    return get_number(str, pos, rev) > 0;
}

char
get_digit(const char *str, size_t pos, int rev) {
    if(isdigit(str[pos])) {
        return str[pos];
    }
    return get_number(str, pos, rev) + '0';
}

size_t
part1(StringArray lines) {
    size_t total = 0;
    for(size_t i = 0; i < lines.count; i++) {
        // printf("Line %lu: " STRING_FMT "\n", i, STRING_ARG(lines.items[i]));
        size_t s   = 0;
        size_t e   = lines.items[i].count - 1;
        char * str = lines.items[i].items;
        while(s <= e && !isdigit(str[s])) {
            ++s;
        }
        while(e >= s && !isdigit(str[e])) {
            --e;
        }
        char   c1  = (str[s] - '0');
        char   c2  = (str[e] - '0');
        size_t num = c1 * 10 + c2;
        // printf(" - Iteration %lu = %c, %lu = %c: %lu \n", s, c1, e, c2, num);
        total += num;
    }

    return total;
}

size_t
part2(StringArray lines) {
    size_t total = 0;
    for(size_t i = 0; i < lines.count; i++) {
        // printf("Line %lu: " STRING_FMT "\n", i, STRING_ARG(lines.items[i]));

        size_t s   = 0;
        size_t e   = lines.items[i].count - 1;
        char * str = lines.items[i].items;
        while(s <= e && !check_digit(str, s, 0)) {
            ++s;
        }
        while(e >= s && !check_digit(str, e, 1)) {
            --e;
        }
        char   c1  = (get_digit(str, s, 0) - '0');
        char   c2  = (get_digit(str, e, 1) - '0');
        size_t num = c1 * 10 + c2;
        // printf(" - Iteration %lu = %c, %lu = %c: %lu \n", s, c1, e, c2, num);
        total += num;
    }

    return total;
}

int
main(int argc, char **argv) {
    CharArray   content = read_file_content("assets/input01.txt");
    StringArray lines   = string_split(content, '\n');

    size_t p1 = part1(lines);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(lines);
    printf(" Part 2 total:  %lu\n", p2);

    AC_ARRAY_DESTROY(lines);
    AC_ARRAY_DESTROY(content);
    return 0;
}
