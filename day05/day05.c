#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <assert.h>

typedef struct range_struct
{
    size_t start;
    size_t len;
    int offset;
} Range;
AC_ARRAY_DEFINE(Range);
AC_ARRAY_DEFINE(RangeArray);

typedef enum map_type
{
    seed_to_soil,
    soil_to_fertilizer,
    fertilizer_to_water,
    water_to_light,
    light_to_temperature,
    temperature_to_humidity,
    humidity_to_location,
    map_type_count
} MapType;

char *map_names[] = {
    "seed-to-soil",
    "soil-to-fertilizer",
    "fertilizer-to-water",
    "water-to-light",
    "light-to-temperature",
    "temperature-to-humidity",
    "humidity-to-location"};

size_t get_map_idx_by_name(String name)
{
    for (size_t i = 0; i < map_type_count; i++)
    {
        if (string_starts_with(name, string_create(map_names[i])))
        {
            return i;
        }
    }
    return STRING_NPOS;
}

void parse_input(StringArray lines, IntegerArray *seeds, RangeArrayArray *maps)
{
    String seeds_line = lines.items[0];
    parser_consume_chars(&seeds_line, 7);
    *seeds = parse_integer_list(seeds_line);

    *maps = AC_ARRAY_CREATE(RangeArray, (int)map_type_count);
    for (size_t i = 0; i < map_type_count; i++)
    {
        AC_ARRAY_INSERT(*maps, (AC_ARRAY_CREATE(Range, 40)));
    }

    size_t l = 1;
    while (lines.count > l && string_ends_with(lines.items[l], string_create(" map:")))
    {
        size_t map_idx = get_map_idx_by_name(lines.items[l]);
        l++;
        while (lines.count > l && !string_ends_with(lines.items[l], string_create(" map:")))
        {
            String current = lines.items[l];
            l++;

            if (current.count == 0)
            {
                continue;
            }

            IntegerArray list = parse_integer_list(current);
            if (list.count != 3)
            {
                printf("Parse error, expected range items... Found %lu integers\n", list.count);
                exit(-1);
            }
            AC_ARRAY_PUSH(maps->items[map_idx], ((Range){.start = list.items[1], .len = list.items[2], .offset = ((int)list.items[0] - (int)list.items[1])}));
            AC_ARRAY_DESTROY(list);
        }
    }
}

void dump_input(IntegerArray *seeds, RangeArrayArray *maps)
{
    printf("Seeds :");
    for (size_t i = 0; i < seeds->count; i++)
    {
        printf(" %lu", seeds->items[i]);
    }
    printf("\n");

    for (size_t m = 0; m < maps->count; m++)
    {
        printf("map '%s': (%lu ranges)\n", map_names[m], maps->items[m].count);
        for (size_t j = 0; j < maps->items[m].count; j++)
        {
            printf("range %lu: start=%lu, len=%lu, offset=%d\n", j, maps->items[m].items[j].start, maps->items[m].items[j].len, maps->items[m].items[j].offset);
        }
    }
}

void release_input(IntegerArray *seeds, RangeArrayArray *maps)
{
    AC_ARRAY_DESTROY(*seeds);
    for (size_t m = 0; m < maps->count; m++)
    {
        AC_ARRAY_DESTROY(maps->items[m]);
    }
    AC_ARRAY_DESTROY(*maps);
}

size_t get_map_value(size_t v, RangeArray map)
{
    for (size_t i = 0; i < map.count; i++)
    {
        if (v >= map.items[i].start && v <= (map.items[i].start + map.items[i].len))
        {
            return v + map.items[i].offset;
        }
    }
    return v;
}

Range get_left(Range a, Range b)
{
    Range left = {0};
    if (a.start < b.start)
    {
        size_t start = min(a.start, b.start);
        size_t end = max(a.start, b.start);
        left = (Range){.start = start, .len = end - start, .offset = 0};
    }
    return left;
}

Range get_mid(Range a, Range b)
{
    Range mid = {0};
    size_t start = max(a.start, b.start);
    size_t end = min(a.start + a.len, b.start + b.len);
    mid = (Range){.start = start, .len = end - start, .offset = 0};
    return mid;
}

Range get_right(Range a, Range b)
{
    Range right = {0};
    if (a.start + a.len > b.start + b.len)
    {
        size_t start = min(a.start + a.len, b.start + b.len);
        size_t end = max(a.start + a.len, b.start + b.len);
        right = (Range){.start = start, .len = end - start, .offset = 0};
    }
    return right;
}

size_t
part1(StringArray lines, IntegerArray seeds, RangeArrayArray maps)
{
    size_t total = STRING_NPOS;
    for (size_t s = 0; s < seeds.count; s++)
    {
        size_t val = seeds.items[s];
        for (size_t m = 0; m < maps.count; m++)
        {
            size_t tmp = get_map_value(val, maps.items[m]);
            val = tmp;
        }
        total = (val < total) ? val : total;
    }
    return total;
}

size_t
part2(StringArray lines, IntegerArray seeds, RangeArrayArray maps)
{

    // // BRUTE FORCE: Too slow, think another way!
    // size_t total = STRING_NPOS;
    // for (size_t s = 0; s < seeds.count; s+=2)
    // {
    //     size_t s_start = seeds.items[s];
    //     size_t s_len = seeds.items[s+1];
    //     for (size_t s = s_start; s < s_start + s_len; s++)
    //     {
    //         size_t val = s;
    //         for (size_t m = 0; m < maps.count; m++)
    //         {
    //             size_t tmp = get_map_value(val, maps.items[m]);
    //             val = tmp;
    //         }
    //         total = (val < total) ? val : total;
    //     }
    // }

    // We will iterate over the ranges and process them, splitting the ranges at each level.
    size_t total = STRING_NPOS;
    RangeArray ranges_bkp = AC_ARRAY_CREATE(Range, seeds.count / 2);
    RangeArray ranges = AC_ARRAY_CREATE(Range, seeds.count / 2);

    // initialize the original ranges
    for (size_t s = 0; s < seeds.count; s += 2)
    {
        size_t s_start = seeds.items[s];
        size_t s_len = seeds.items[s + 1];
        AC_ARRAY_PUSH(ranges, ((Range){.start = s_start, .len = s_len, .offset = 0}));
    }

    // iterate over each map
    for (size_t m = 0; m < maps.count; m++)
    {
        // process each seed range
        while (ranges.count > 0)
        {
            Range range = {0};
            AC_ARRAY_POP(ranges, range);
            assert(range.len > 0);
            size_t backup_count = ranges_bkp.count;

            // every range on the current map
            for (size_t r = 0; r < maps.items[m].count; r++)
            {
                Range a = range;
                Range b = maps.items[m].items[r];

                if (a.start + a.len <= b.start || b.start + b.len <= a.start)
                {
                    continue;
                }

                // split the range
                Range left = get_left(a, b);
                Range mid = get_mid(a, b);
                Range right = get_right(a, b);
                assert(mid.len > 0);

                if (left.len > 0)
                {
                    AC_ARRAY_PUSH(ranges, left);
                }
                if (right.len > 0)
                {
                    AC_ARRAY_PUSH(ranges, right);
                }

                mid.start += b.offset;
                AC_ARRAY_PUSH(ranges_bkp, mid);
                break;
            }

            // if no collision with ranges, push it as is.
            if(backup_count == ranges_bkp.count)
            {
                AC_ARRAY_PUSH(ranges_bkp, range);
            }
        }

        RangeArray ranges_tmp = ranges;
        ranges = ranges_bkp;
        ranges_bkp = ranges_tmp;
        ranges_bkp.count = 0;
    }

    for (size_t i = 0; i < ranges.count; i++)
    {
        total = min(total, ranges.items[i].start);
    }

    AC_ARRAY_DESTROY(ranges_bkp);
    AC_ARRAY_DESTROY(ranges);

    return total;
}

int main(int argc, char **argv)
{
    CharArray content = read_file_content("assets/input01.txt");
    StringArray lines = string_split(content, '\n');

    IntegerArray seeds;
    RangeArrayArray maps;
    parse_input(lines, &seeds, &maps);
    // dump_input(&seeds, &maps);

    size_t p1 = part1(lines, seeds, maps);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(lines, seeds, maps);
    printf(" Part 2 total:  %lu\n", p2);

    release_input(&seeds, &maps);
    AC_ARRAY_DESTROY(lines);
    AC_ARRAY_DESTROY(content);
    return 0;
}
