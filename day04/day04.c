#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>


size_t
part1(StringArray lines) {
    size_t total = 0;
    for (size_t i = 0; i < lines.count; i++)
    {
        String data = lines.items[i];
        StringArray tmp = string_split(data, ':');
        StringArray lists = string_split(tmp.items[1], '|');

        IntegerArray win = parse_integer_list(string_trim(lists.items[0]));
        IntegerArray candidates = parse_integer_list(string_trim(lists.items[1]));

        size_t val = 0;
        for (size_t w = 0; w < win.count; w++)
        {
            for (size_t c = 0; c < candidates.count; c++)
            {
                if(win.items[w] == candidates.items[c]) {
                    val = (val == 0) ? 1 : val *2;
                    // printf("Card %lu: val=%lu <-- found: %lu\n", i, val, candidates.items[c]);
                    break;
                }
            }
        }
        // printf("Card %lu: val=%lu\n", i, val);
        total += val;
        AC_ARRAY_DESTROY(win);
        AC_ARRAY_DESTROY(candidates);
    }

    return total;
}

size_t
part2(StringArray lines) {

    IntegerArray copies = AC_ARRAY_CREATE(Integer, lines.count);
    AC_ARRAY_CLEAN(copies);

    size_t total = 0;
    for (size_t i = 0; i < lines.count; i++)
    {
        String data = lines.items[i];
        StringArray tmp = string_split(data, ':');
        StringArray lists = string_split(tmp.items[1], '|');

        IntegerArray win = parse_integer_list(string_trim(lists.items[0]));
        IntegerArray candidates = parse_integer_list(string_trim(lists.items[1]));

        size_t val = 0;
        for (size_t w = 0; w < win.count; w++)
        {
            for (size_t c = 0; c < candidates.count; c++)
            {
                if(win.items[w] == candidates.items[c]) {
                    val++;
                    break;
                }
            }
        }
        
        
        size_t instances = 1 + copies.items[i];
        for (size_t j = 0; j < val; j++)
        {
            copies.items[i + j + 1] += instances;
        }
        // printf("Card %lu: val=%lu, current=%d, instances=%lu\n", i+1, val, 1, copies.items[i]);
        total += instances;
        
        AC_ARRAY_DESTROY(win);
        AC_ARRAY_DESTROY(candidates);
    }

    AC_ARRAY_DESTROY(copies);
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
