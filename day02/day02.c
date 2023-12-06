#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

size_t
solve(StringArray lines)
{
    size_t total_part1 = 0;
    size_t total_part2 = 0;
    for (size_t i = 0; i < lines.count; i++)
    {
        String data = lines.items[i];

        parser_consume_chars(&data, 5); // ignore "Game "

        size_t id = parser_consume_int(&data, NULL); // get id

        parser_consume_chars(&data, 1); // ignore ":"

        size_t max_r = 0;
        size_t max_g = 0;
        size_t max_b = 0;

        StringArray parts = string_split(data, ';');
        for (size_t p = 0; p < parts.count; p++)
        {
            StringArray cubes = string_split(parts.items[p], ',');
            for (size_t c = 0; c < cubes.count; c++)
            {
                String cube = cubes.items[c];
                parser_consume_chars(&cube, 1); // ignore " "

                size_t count = parser_consume_int(&cube, NULL); // get amount

                parser_consume_chars(&cube, 1); // ignore " "

                String color = parser_consume_literal(&cube); // get color
                if(string_equals(color, string_create("red"))) {
                    max_r = max_r > count ? max_r : count;
                } else if(string_equals(color, string_create("green"))) {
                    max_g = max_g > count ? max_g : count;
                } else if(string_equals(color, string_create("blue"))) {
                    max_b = max_b > count ? max_b : count;
                } 
            }
            AC_ARRAY_DESTROY(cubes);
        }
        AC_ARRAY_DESTROY(parts);
        //printf("id: %lu , r: %lu, g: %lu, b: %lu\n", id, max_r, max_g, max_b);

        if (max_r <= 12 && max_g <= 13 && max_b <= 14)
        {
            total_part1 += id;
        }
        total_part2 += max_r * max_g * max_b;
    }
    
    printf(" Part 1 total:  %lu\n", total_part1);
    printf(" Part 2 total:  %lu\n", total_part2);
}

int main(int argc, char **argv)
{
    CharArray content = read_file_content("assets/input01.txt");
    StringArray lines = string_split(content, '\n');

    solve(lines);

    AC_ARRAY_DESTROY(lines);
    AC_ARRAY_DESTROY(content);
    return 0;
}
