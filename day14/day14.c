#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>



void
dump(StringArray map, size_t xpos, size_t ypos) 
{
    size_t h = map.count;
    for (size_t y = 0; y < h; y++)
    {
        printf("%c %3lu: " STRING_FMT "\n", (ypos == y) ? '>' : ' ' ,h - y, STRING_ARG(map.items[y]));
    }
    printf("       ");
    for (size_t x = 0; x < xpos; x++)
    {
        printf(" ");
    }

    printf("^\n");
}


void
move_north(StringArray map) 
{
    size_t w = map.items[0].count - 1;
    size_t h = map.count - 1;

    for (size_t y = 1; y <= h; y++)
    {
        for (size_t x = 0; x <= w; x++)
        {
            if(map.items[y].items[x] == 'O' && map.items[y - 1].items[x] == '.')
            {
                size_t ypos = y - 1;
                while(ypos > 0 && map.items[ypos].items[x] == '.') {
                    ypos--;
                }
                if(map.items[ypos].items[x] != '.') {
                    ypos++;
                }
                map.items[ypos].items[x] = 'O';
                map.items[y].items[x] = '.';
            }
        }
    }
}

void
move_south(StringArray map) 
{
    size_t w = map.items[0].count - 1;
    size_t h = map.count - 1;

    for (size_t y = h-1; y >= 0 && y <= h; y--)
    {
        for (size_t x = 0; x <= w; x++)
        {
            if(map.items[y].items[x] == 'O' && map.items[y + 1].items[x] == '.')
            {
                size_t ypos = y + 1;
                while(ypos < h && map.items[ypos].items[x] == '.') {
                    ypos++;
                }
                if(map.items[ypos].items[x] != '.') {
                    ypos--;
                }
                map.items[ypos].items[x] = 'O';
                map.items[y].items[x] = '.';
            }
        }
    }
}
void
move_west(StringArray map) 
{
    size_t w = map.items[0].count - 1;
    size_t h = map.count - 1;

    for (size_t x = 1; x <= w; x++)
    {
        for (size_t y = 0; y <= h; y++)
        {
            if(map.items[y].items[x] == 'O' && map.items[y].items[x - 1] == '.')
            {
                size_t xpos = x - 1;
                while(xpos > 0 && map.items[y].items[xpos] == '.') {
                    xpos--;
                }
                if(map.items[y].items[xpos] != '.') {
                    xpos++;
                }
                map.items[y].items[xpos] = 'O';
                map.items[y].items[x] = '.';
            }
        }
    }
}

void
move_east(StringArray map)
{
    size_t w = map.items[0].count - 1;
    size_t h = map.count - 1;

    for (size_t x = w-1; x >= 0 && x <= w; x--)
    {
        for (size_t y = 0; y <= h; y++)
        {
            if(map.items[y].items[x] == 'O' && map.items[y].items[x + 1] == '.')
            {
                size_t xpos = x + 1;
                while(xpos < w && map.items[y].items[xpos] == '.') {
                    xpos++;
                }
                if(map.items[y].items[xpos] != '.') {
                    xpos--;
                }
                map.items[y].items[xpos] = 'O';
                map.items[y].items[x] = '.';
            }
        }
    }
}

void
cycle(StringArray map)
{
    move_north(map);
    move_west(map);
    move_south(map);
    move_east(map);
}

size_t
count(StringArray map)
{
    size_t ret = 0;
    size_t h = map.count;
    for (size_t y = 0; y < h; y++)
    {
        ret += (h - y) * string_count(map.items[y], 'O');
    }
    
    return ret;
}

typedef struct cycle_struct {
    size_t offset;
    size_t len;
} Cycle;

bool
validate_cycle(IntegerArray list, Cycle cycle)
{
    bool found = false;
    if(list.count > cycle.offset + cycle.len*2)
    {
        found = true;
        for (size_t i = 0; i < cycle.len; i++)
        {
            if(list.items[cycle.offset + i] != list.items[cycle.offset + i + cycle.len])
            {
                found = false;
                break;
            }
        }
    }
    return found;
}

Cycle
find_cycle(IntegerArray list)
{
    Cycle c;
    for (size_t i = list.count / 2; i > 1; i--)
    {
        c.len = i;
        for (size_t p = 0; p < list.count - i; p++)
        {
            c.offset = p;
            if(validate_cycle(list, c))
            {
                return c;
            }
        }
    }
    return (Cycle){0};
}


size_t
part1(StringArray map)
{
    move_north(map);
    return count(map);
}

size_t
part2(StringArray map)
{
    size_t i = 0;
    size_t max = 1000000000;
    size_t limit = 100;
    size_t tortoise = 0;
    size_t hare = 1;
    size_t score = 2;
    IntegerArray list = AC_ARRAY_CREATE(Integer, 100);

    Cycle c;
    do
    {    
        while(i < limit)
        {
            cycle(map);
            ++i;
            score = count(map);
            AC_ARRAY_PUSH(list, score);

            // printf("%lu: %lu\n", i, score);

            c = find_cycle(list);
            if(c.len > 0) {
                break;
            }
        }
        if(c.len > 0) {
            size_t pos_value = ((max - c.offset) % c.len) + c.offset - 1;
            printf("%lu: Found cycle %lu, %lu\n", i, c.offset, c.len);
            printf("pos value %lu: %lu\n", pos_value, list.items[pos_value]);
            score =  list.items[pos_value];
        } else {
            limit *= 2;
        }
    } while (c.len == 0);

    AC_ARRAY_DESTROY(list);
    return score;
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
