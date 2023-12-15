#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

size_t
solve_pattern(StringArray map, bool have_smudge)
{
    size_t w = map.items[0].count;
    size_t h = map.count;

    // for each position try to get the maximum symmetries
    size_t total = 0;
    for (size_t x = 0; x < w - 1; x++)
    {
        size_t error = 0;
        for (size_t i = 0; i <= w ; i++)
        {
            size_t l = x - i;
            size_t r = x + 1 + i;
            if(l >= w || r >= w)
            {
                break;
            }
            for (size_t y = 0; y < h; y++)
            {
                if(map.items[y].items[l] != map.items[y].items[r]) {
                    error++;
                }
            }
        }
        if(error == (have_smudge ? 1 : 0)) {
            total += (x + 1);
        }
    }

    // Repeat for horizontal!
    for (size_t y = 0; y < h - 1; y++)
    {
        size_t error = 0;
        for (size_t i = 0; i <= h ; i++)
        {
            size_t t = y - i;
            size_t b = y + 1 + i;
            if(t >= h || b >= h)
            {
                break;
            }
            for (size_t x = 0; x < w; x++)
            {
                if(map.items[t].items[x] != map.items[b].items[x]) {
                    error++;
                }
            }
        }
        if(error == (have_smudge ? 1 : 0)) {
            total += (y + 1) * 100;
        }
    }

    return total;
}


size_t
solve(StringArray lines, bool have_smudge)
{

    size_t total = 0;
    size_t pos = 0;
    while(pos < lines.count)
    {
        // count pattern block size
        size_t start = pos;
        while (lines.items[pos].count > 0 && pos < lines.count)
        {
            pos++;
        }

        // build pattern block
        size_t len = pos - start;
        StringArray pattern = AC_ARRAY_CREATE(String, len);
        for (size_t i = start; i < pos; i++)
        {
            AC_ARRAY_INSERT(pattern, lines.items[i]);
        }
        pos++;  // skip empty line

        // solve it
        size_t pattern_score = solve_pattern(pattern, have_smudge);
        // printf("score: %lu\n\n", pattern_score);
        total += pattern_score;

        AC_ARRAY_DESTROY(pattern);
    }
    return total;
}

size_t
part1(StringArray lines)
{
    return solve(lines, false);
}

size_t
part2(StringArray lines)
{
    return solve(lines, true);
}

int main(int argc, char **argv)
{
    CharArray content = read_file_content("assets/input01.txt");
    StringArray groups = string_split_ex(content, '\n', true);

    size_t p1 = part1(groups);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(groups);
    printf(" Part 2 total:  %lu\n", p2);

    AC_ARRAY_DESTROY(groups);
    AC_ARRAY_DESTROY(content);
    return 0;
}
