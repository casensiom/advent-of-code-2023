#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

size_t
hash(String str)
{
    size_t hash = 0;
    for (size_t i = 0; i < str.count; i++)
    {
        hash += str.items[i];
        hash *= 17;
        hash = hash % 256;
    }
    return hash;
}

size_t
part1(StringArray lines)
{
    size_t total = 0;
    for (size_t i = 0; i < lines.count; i++)
    {
        total += hash(lines.items[i]);
    }

    return total;
}

typedef struct lens_struct
{
    String label;
    size_t val;
} Lens;
AC_ARRAY_DEFINE(Lens);

void process_hash(LensArray *hashmap, String inst)
{

    if (inst.items[inst.count - 1] == '-')
    {
        String label = inst;
        label.count--;
        size_t hash_pos = hash(label);
        for (size_t i = 0; i < hashmap[hash_pos].count; i++)
        {
            if (string_equals(hashmap[hash_pos].items[i].label, label))
            {
                AC_ARRAY_REMOVE(hashmap[hash_pos], i);
                break;
            }
        }
    }
    else
    {
        StringArray parts = string_split(inst, '=');
        String label = parts.items[0];
        String number = parts.items[1];
        size_t val = parser_consume_int(&number, NULL);
        size_t hash_pos = hash(label);

        size_t pos = STRING_NPOS;
        for (size_t i = 0; i < hashmap[hash_pos].count; i++)
        {
            if (string_equals(hashmap[hash_pos].items[i].label, label))
            {
                pos = i;
                break;
            }
        }

        if (pos != STRING_NPOS)
        {
            hashmap[hash_pos].items[pos].val = val;
        }
        else
        {
            AC_ARRAY_PUSH(hashmap[hash_pos], ((Lens){.label = label, .val = val}));
        }

        AC_ARRAY_DESTROY(parts);
    }
}

size_t
part2(StringArray lines)
{
    size_t estimated = max(1, lines.count / 256);
    LensArray hashmap[256];
    for (size_t i = 0; i < 256; i++)
    {
        hashmap[i] = AC_ARRAY_CREATE(Lens, estimated);
    }

    for (size_t i = 0; i < lines.count; i++)
    {
        process_hash(hashmap, lines.items[i]);
    }

    size_t total = 0;
    for (size_t i = 0; i < 256; i++)
    {
        for (size_t j = 0; j < hashmap[i].count; j++)
        {
            size_t partial = (i + 1) * (j + 1) * hashmap[i].items[j].val;
            printf(STRING_FMT ": %lu * %lu * %lu = %lu\n", STRING_ARG(hashmap[i].items[j].label), (i + 1), (j + 1), hashmap[i].items[j].val, partial);
            total += partial;
        }
    }

    for (size_t i = 0; i < 256; i++)
    {
        AC_ARRAY_DESTROY(hashmap[i]);
    }
    return total;
}

int main(int argc, char **argv)
{
    CharArray content = read_file_content("assets/input01.txt");
    StringArray lines = string_split(content, ',');

    size_t p1 = part1(lines);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(lines);
    printf(" Part 2 total:  %lu\n", p2);

    AC_ARRAY_DESTROY(lines);
    AC_ARRAY_DESTROY(content);
    return 0;
}
