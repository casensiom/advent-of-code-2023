#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>


IntegerArray lookup_table;
size_t cache_hits;
size_t cache_miss;

size_t
solve_recursive_with_index(String map, IntegerArray groups, size_t map_pos, size_t group_pos)
{
    size_t key = map_pos + group_pos * map.count;
    if (lookup_table.items[key] != STRING_NPOS)
    {
        // printf("cache: map_pos: %lu, group_pos: %lu, [%lu]-> %lu\n", map_pos, group_pos, key, lookup_table.items[key]);
        cache_hits++;
        return lookup_table.items[key];
    }

    // printf("compute: map_pos: %lu, group_pos: %lu, [%lu]-> %lu\n", map_pos, group_pos, key, lookup_table.items[key]);
    if (map_pos == map.count)
    {
        return (group_pos == groups.count) ? 1 : 0;
    }

    if (group_pos == groups.count)
    {
        size_t pending = string_count_from(map, '#', map_pos);
        return (pending == 0) ? 1 : 0;
    }

    size_t total = 0;
    if (map.items[map_pos] == '.' || map.items[map_pos] == '?')
    {
        total += solve_recursive_with_index(map, groups, map_pos + 1, group_pos);
    }

    if (map.items[map_pos] == '#' || map.items[map_pos] == '?')
    {
        size_t group_size = groups.items[group_pos];
        size_t next_dot = string_find_from(map, string_create("."), map_pos);
        size_t end_group = map_pos + group_size;
        if (end_group <= map.count && // we have enough space on the string for the group
            next_dot >= end_group &&  // the next dot is not on the group
            (end_group == map.count || map.items[end_group] != '#'))
        {
            total += solve_recursive_with_index(map, groups, min(end_group + 1, map.count), group_pos + 1);
        }
    }

    // printf("SET cache: map_pos: %lu, group_pos: %lu -> [%lu] %lu\n", map_pos, group_pos, key, total);
    lookup_table.items[key] = total;
    cache_miss++;

    return total;
}

size_t
solve_fast(String line)
{
    StringArray parts = string_split(line, ' ');

    String str = parts.items[0];
    IntegerArray groups = parse_integer_list_with_delim(parts.items[1], ',');
    size_t max_value = 0;
    for (size_t i = 0; i < groups.count; i++)
    {
        max_value = max(max_value, groups.items[i]);
    }

    lookup_table = AC_ARRAY_CREATE(Integer, (str.count + 1) * (groups.count + 1));
    for (size_t i = 0; i < lookup_table.capacity; i++)
    {
        lookup_table.items[i] = STRING_NPOS;
    }

    cache_hits = 0;
    cache_miss = 0;
    size_t total = solve_recursive_with_index(str, groups, 0, 0);

    // printf("%lu: " STRING_FMT " (hits: %lu, miss: %lu)\n", total, STRING_ARG(str), cache_hits, cache_miss);

    AC_ARRAY_DESTROY(lookup_table);

    AC_ARRAY_DESTROY(groups);
    return total;
}

size_t
part1(StringArray lines)
{
    size_t total = 0;
    size_t i = lines.count - 1;
    for (size_t i = 0; i < lines.count; i++)
    {
        size_t sol = solve_fast(lines.items[i]);
        total += sol;
    }
    return total;
}

size_t
part2(StringArray lines)
{
    size_t total = 0;
    for (size_t i = 0; i < lines.count; i++)
    {
        StringArray parts = string_split(lines.items[i], ' ');
        StringArray springs = AC_ARRAY_CREATE(String, 5);
        StringArray counts = AC_ARRAY_CREATE(String, 5);

        for (size_t c = 0; c < 5; c++)
        {
            AC_ARRAY_INSERT(springs, parts.items[0]);
            AC_ARRAY_INSERT(counts, parts.items[1]);
        }

        String unfold_spring = string_join_with_string(springs, string_create("?"));
        String unfold_count = string_join_with_string(counts, string_create(","));

        String unfold_tmp = string_concat(unfold_spring, string_create(" "));
        String unfold = string_concat(unfold_tmp, unfold_count);

        size_t sol = solve_fast(unfold);
        total += sol;

        AC_ARRAY_DESTROY(unfold);
        AC_ARRAY_DESTROY(unfold_tmp);
        AC_ARRAY_DESTROY(unfold_count);
        AC_ARRAY_DESTROY(unfold_spring);
        AC_ARRAY_DESTROY(springs);
        AC_ARRAY_DESTROY(counts);
        AC_ARRAY_DESTROY(parts);
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
