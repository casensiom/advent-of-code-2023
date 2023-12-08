#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct node_struct
{
    String id;
    String left;
    String right;
} Node;
AC_ARRAY_DEFINE(Node);

size_t find_node(NodeArray nodes, String id)
{
    for (size_t i = 0; i < nodes.count; i++)
    {
        if (string_equals(nodes.items[i].id, id))
        {
            return i;
        }
    }
    return STRING_NPOS;
}

size_t solve1(String instructions, NodeArray nodes)
{
    size_t steps = 0;
    size_t pos = 0;
    size_t node = 0;
    String id;

    node = find_node(nodes, string_create("AAA"));
    while (true)
    {
        steps++;
        if (instructions.items[pos] == 'R')
        {
            id = nodes.items[node].right;
        }
        else
        {
            id = nodes.items[node].left;
        }
        node = find_node(nodes, id);
        if (string_equals(nodes.items[node].id, string_create("ZZZ")))
        {
            break;
        }
        pos++;
        if (pos >= instructions.count)
        {
            pos = 0;
        }
    }

    return steps;
}

size_t compute_gcd(size_t a, size_t b)
{
    while (b != 0) {
        size_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

size_t compute_lcm(size_t a, size_t b)
{
    return (a * b) / compute_gcd(a, b);
}

size_t solve2(String instructions, NodeArray nodes)
{
    size_t steps = 0;
    size_t pos = 0;
    String id;

    size_t counter = 0;
    for (size_t i = 0; i < nodes.count; i++)
    {
        if(string_ends_with(nodes.items[i].id, string_create("A")))
        {
            counter++;
        }
    }
    IntegerArray paths = AC_ARRAY_CREATE(Integer, counter);
    IntegerArray found = AC_ARRAY_CREATE(Integer, counter);
    AC_ARRAY_CLEAN(found);


    for (size_t i = 0; i < nodes.count; i++)
    {
        if(string_ends_with(nodes.items[i].id, string_create("A")))
        {
            AC_ARRAY_INSERT(paths, i);
        }
    }

    while (true)
    {
        steps++;
        bool all_ends = true;
        for (size_t i = 0; i < paths.count; i++)
        {
            size_t node_pos = paths.items[i];
            if (instructions.items[pos % instructions.count] == 'R')
            {
                id = nodes.items[node_pos].right;
            }
            else
            {
                id = nodes.items[node_pos].left;
            }
            paths.items[i] = find_node(nodes, id);

            if (string_ends_with(id, string_create("Z")))
            {
                if(found.items[i] == 0)
                {
                    // printf("Path %lu, arrives to Z at %lu\n", i, steps);
                    found.items[i] = steps;
                    found.count++;
                }

                if(found.count == paths.count) 
                {
                    size_t lcm = 1;
                    for (size_t i = 0; i < found.count; i++)
                    {
                        lcm = compute_lcm(lcm, found.items[i]);
                    }
                    // printf("lcm %lu\n", lcm);
                    return lcm;
                }
            }
        }
        pos++;
    }

    AC_ARRAY_DESTROY(paths);
    AC_ARRAY_DESTROY(found);

    return steps;
}

size_t
part1(StringArray lines)
{
    String instructions = lines.items[0];

    NodeArray nodes = AC_ARRAY_CREATE(Node, lines.count - 1);
    for (size_t i = 1; i < lines.count; i++)
    {
        String item = lines.items[i];
        String id = parser_consume_chars(&item, 3);
        parser_consume_chars(&item, 4);
        String left = parser_consume_chars(&item, 3);
        parser_consume_chars(&item, 2);
        String right = parser_consume_chars(&item, 3);

        AC_ARRAY_INSERT(nodes, ((Node){.id = id, .left = left, .right = right}));
    }

    size_t steps = solve1(instructions, nodes);

    AC_ARRAY_DESTROY(nodes);
    return steps;
}

size_t
part2(StringArray lines)
{
    String instructions = lines.items[0];

    NodeArray nodes = AC_ARRAY_CREATE(Node, lines.count - 1);
    for (size_t i = 1; i < lines.count; i++)
    {
        String item = lines.items[i];
        String id = parser_consume_chars(&item, 3);
        parser_consume_chars(&item, 4);
        String left = parser_consume_chars(&item, 3);
        parser_consume_chars(&item, 2);
        String right = parser_consume_chars(&item, 3);

        AC_ARRAY_INSERT(nodes, ((Node){.id = id, .left = left, .right = right}));
    }

    size_t steps = solve2(instructions, nodes);

    AC_ARRAY_DESTROY(nodes);
    return steps;
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
