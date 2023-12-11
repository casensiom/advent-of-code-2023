#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct point2d_struct {
    size_t x;
    size_t y;
} Point2D;
AC_ARRAY_DEFINE(Point2D);


void
dump(StringArray map, IntegerArray horizontal, IntegerArray vertical)
{
    size_t w = map.items[0].count;
    size_t h = map.count;
    for (size_t y = 0; y < h; y++)
    {
        for (size_t x = 0; x < w; x++)
        {
            printf("%c", map.items[y].items[x]);
        }
        printf(": %lu\n", horizontal.items[y]);
    }
    for (size_t x = 0; x < w; x++)
    {
        printf("%lu", vertical.items[x]);
    }
    printf("\n");
}

size_t
solve(StringArray map, size_t dist)
{
    size_t w = map.items[0].count;
    size_t h = map.count;

    IntegerArray horizontal = AC_ARRAY_CREATE(Integer, h);
    IntegerArray vertical = AC_ARRAY_CREATE(Integer, w);
    Point2DArray galaxies = AC_ARRAY_CREATE(Point2D, h*w);

    size_t count = 0;
    for (size_t x = 0; x < w; x++)
    {
        bool empty = true;
        for (size_t y = 0; y < h; y++)
        {
            if(map.items[y].items[x] == '#')
            {
                empty = false;
                break;
            }
        }
        count += (empty) ? 1 : 0;
        vertical.items[x] = count;
    }
    

    count = 0;
    for (size_t y = 0; y < h; y++)
    {
        count += (string_count(map.items[y], '#') == 0) ? 1 : 0;
        horizontal.items[y] = count;
    }

    for (size_t y = 0; y < h; y++)
    {
        for (size_t x = 0; x < w; x++)
        {
            if(map.items[y].items[x] == '#')
            {
                AC_ARRAY_INSERT(galaxies, ((Point2D){.x = x, .y = y}));
            }
        }
    }

    // dump(map, horizontal, vertical);

    size_t total = 0;
    for (size_t g1 = 0; g1 < galaxies.count; g1++)
    {
        for (size_t g2 = g1 + 1; g2 < galaxies.count; g2++)
        {
            size_t p1x = galaxies.items[g1].x + vertical.items[galaxies.items[g1].x] * dist;
            size_t p1y = galaxies.items[g1].y + horizontal.items[galaxies.items[g1].y] * dist;
            size_t p2x = galaxies.items[g2].x + vertical.items[galaxies.items[g2].x] * dist;
            size_t p2y = galaxies.items[g2].y + horizontal.items[galaxies.items[g2].y] * dist;

            size_t dx = abs(p2x - p1x);
            size_t dy = abs(p2y - p1y);

            size_t d = dx + dy;
            total += d;
        }
    }

    AC_ARRAY_DESTROY(horizontal);
    AC_ARRAY_DESTROY(vertical);
    AC_ARRAY_DESTROY(galaxies);

    return total;
}


size_t
part1(StringArray lines)
{
    return solve(lines, 1);
}

size_t
part2(StringArray lines)
{
    return solve(lines, 1000000 - 1);
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
