#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>


int
is_digit(char v) {
    return (v >= '0' && v <= '9');
}

int
is_symbol(char v) {
    return (v != '.' && !is_digit(v));
}

size_t 
search_first_digit(String data, size_t pos)
{
    size_t start = data.count;
    for (size_t x = pos; x < data.count; x++)
    {
        if(is_digit(data.items[x])) {
            start = x;
            break;
        }
    }
    return start;
}

int
check_surround(StringArray lines, size_t l, size_t start, size_t len) {
    size_t count = lines.items[l].count;

    for (int y = -1; y <= 1; y++)
    {

        size_t line = l + y;
        if(line >= lines.count) {
            continue;
        }

        for (int x = -1; x < (int)(len + 1); x++)
        {
            size_t chr = start + x;
            if(chr >= lines.items[line].count) {
                continue;
            }

            if(is_symbol(lines.items[line].items[chr]))
            {
                return 1;
            }
        }
    }
    return 0; 
}

size_t
part1(StringArray lines) {

    size_t total = 0;
    for (size_t y = 0; y < lines.count; y++)
    {
        String data = lines.items[y];
        // printf("Line: %lu | " STRING_FMT "\n", y, STRING_ARG(data) );

        size_t pos = 0;
        do {
            pos = search_first_digit(data, pos);

            if(pos < data.count) {
                char *ptr_start = data.items + pos;
                char *ptr_end = ptr_start;
                size_t val = strtoll(ptr_start, &ptr_end, 10);
                size_t len = ptr_end - ptr_start;
                // printf("Found val: %lu at %lu\n", val, pos);
                if(check_surround(lines, y, pos, len)) {
                    // printf("   Using val: %lu\n", val);
                    total += val;
                }
                pos += len + 1;
            }
        } while(pos < data.count);
    }
    return total;
}

typedef struct adjacent_struct {
    size_t val;
    size_t count;
} Adjacent;
AC_ARRAY_DEFINE(Adjacent);


void
acc_surround(AdjacentArray *arr, StringArray lines, size_t l, size_t start, size_t len, size_t val) {
    size_t count = lines.items[l].count;

    for (int y = -1; y <= 1; y++)
    {

        size_t line = l + y;
        if(line >= lines.count) {
            continue;
        }

        for (int x = -1; x < (int)(len + 1); x++)
        {
            size_t chr = start + x;
            if(chr >= lines.items[line].count) {
                continue;
            }

            if(lines.items[line].items[chr] == '*')
            {
                if(arr->items[line * count + chr].val == 0) {
                    arr->items[line * count + chr].val = 1;
                }
                arr->items[line * count + chr].count++;
                arr->items[line * count + chr].val *= val;
                // printf("Use value: %lu at (%lu, %lu) [count: %lu]\n", val, line, chr, arr->items[line * count + chr].count);
            }
        }
    }
}


size_t
part2(StringArray lines) {
    size_t max_symbols = lines.count * lines.items[0].count;
    AdjacentArray arr = AC_ARRAY_CREATE(Adjacent, max_symbols);
    AC_ARRAY_CLEAN(arr);

    size_t total = 0;
    for (size_t y = 0; y < lines.count; y++)
    {
        String data = lines.items[y];
        // printf("Line: %lu | " STRING_FMT "\n", y, STRING_ARG(data) );

        size_t pos = 0;
        do {
            pos = search_first_digit(data, pos);

            if(pos < data.count) {
                char *ptr_start = data.items + pos;
                char *ptr_end = ptr_start;
                size_t val = strtoll(ptr_start, &ptr_end, 10);
                size_t len = ptr_end - ptr_start;

                acc_surround(&arr, lines, y, pos, len, val);
                pos += len + 1;
            }
        } while(pos < data.count);
    }

    for (size_t i = 0; i < arr.capacity; i++)
    {
        if(arr.items[i].count == 2) {
            total += arr.items[i].val;
        }
    }
    

    AC_ARRAY_DESTROY(arr);
    return total;
}

int
main(int argc, char **argv) {
    CharArray   content = read_file_content("assets/input01.txt");
    StringArray lines   = string_split(content, '\n');

    size_t p1 = part1(lines);
    printf(" Part 1 total:  %lu\n", p1);

    // size_t max_symbols = string_count(content, '*');
    size_t p2 = part2(lines);
    printf(" Part 2 total:  %lu\n", p2);

    AC_ARRAY_DESTROY(lines);
    AC_ARRAY_DESTROY(content);
    return 0;
}
