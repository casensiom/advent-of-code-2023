#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

size_t solve_race(size_t time, size_t dist)
{
    size_t ret = 0;
    for (size_t i = 0; i < time; i++)
    {
        if ((i * (time - i)) > dist)
        {
            ret++;
        }
    }

    return ret;
}

size_t solve_race_fast(size_t time, size_t dist)
{
    // it could be a binary search to make it faster
    size_t i = 0;
    while ((i * (time - i)) <= dist)
    {
        i++;
    }

    size_t j = (time - i);
    // // Since the curve is symetric the right value dont need to be calculated!
    // size_t j = time;
    // while ((j * (time - j)) <= dist)
    // {
    //     j--;
    // }
    // printf("t: %lu, d: %lu | l: %lu - r: %lu (%lu)\n", time, dist, i, j, (time - i));

    return (j - i) + 1;
}

size_t
part1(StringArray lines)
{
    String time_str = lines.items[0];
    String dist_str = lines.items[1];

    parser_consume_chars(&time_str, 10);
    parser_consume_chars(&dist_str, 10);

    IntegerArray times = parse_integer_list(time_str);
    IntegerArray dists = parse_integer_list(dist_str);

    assert(times.count == dists.count);

    size_t total = 1;
    for (size_t i = 0; i < times.count; i++)
    {
        total *= solve_race_fast(times.items[i], dists.items[i]);
    }

    AC_ARRAY_DESTROY(times);
    AC_ARRAY_DESTROY(dists);

    return total;
}

size_t
part2(StringArray lines)
{
    String time_str = lines.items[0];
    String dist_str = lines.items[1];

    parser_consume_chars(&time_str, 10);
    parser_consume_chars(&dist_str, 10);

    string_remove(&time_str, ' ');
    string_remove(&dist_str, ' ');

    size_t time = parser_consume_int(&time_str, NULL);
    size_t dist = parser_consume_int(&dist_str, NULL);

    printf("time: %lu, dist: %lu\n", time, dist);
    size_t total = solve_race_fast(time, dist);

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
