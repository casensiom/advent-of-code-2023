#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum
{
    RIGHT = 1,
    BOTTOM = 2,
    LEFT = 4,
    UP = 8,
    NUM_DIRECTIONS = 4
} Direction;

typedef struct beam_struct
{
    size_t x;
    size_t y;
    size_t dir;
    bool active;
} Beam;
AC_ARRAY_DEFINE(Beam);

size_t
count_active_beams(BeamArray beams)
{
    size_t count = 0;
    for (size_t i = 0; i < beams.count; i++)
    {
        count += (beams.items[i].active) ? 1 : 0;
    }
    return count;
}

void dump_steps(StringArray map, IntegerArray steps, size_t h, size_t w, BeamArray beams)
{
    size_t ret = 0;
    size_t actives = 0;
    for (size_t y = 0; y < h; y++)
    {
        for (size_t x = 0; x < w; x++)
        {
            bool found = false;
            if (map.items[y].items[x] == '.')
            {
                for (size_t i = 0; i < beams.count; i++)
                {
                    if (beams.items[i].active && beams.items[i].x == x && beams.items[i].y == y)
                    {
                        size_t dir = beams.items[i].dir;
                        printf("%c", (dir == RIGHT) ? '>' : (dir == UP) ? '^'
                                                        : (dir == LEFT) ? '<'
                                                                        : 'v');
                        found = true;
                        actives++;
                        break;
                    }
                }
                if (!found)
                {
                    if (steps.items[y * w + x] != 0)
                    {
                        // printf("%lu", steps.items[y * w + x]);
                        printf("#");
                    }
                    else
                    {
                        printf(".");
                    }
                }
            }
            else
            {
                printf("%c", map.items[y].items[x]);
            }
        }
        printf("\n");
    }
    printf("Active beams: %lu.\n", actives);
}

size_t
solve(StringArray map, Beam start)
{
    BeamArray beams = AC_ARRAY_CREATE(Beam, 10);
    AC_ARRAY_INSERT(beams, start);

    size_t w = map.items[0].count;
    size_t h = map.count;
    IntegerArray steps = AC_ARRAY_CREATE(Integer, h * w);
    AC_ARRAY_CLEAN(steps);

    while (count_active_beams(beams) > 0)
    {
        for (size_t i = 0; i < beams.count; i++)
        {
            Beam *b = &beams.items[i];
            if (b->active == false)
            {
                continue;
            }
            if ((steps.items[b->y * w + b->x] & b->dir) != 0)
            {
                b->active = false;
                continue;
            }

            char action = map.items[b->y].items[b->x];
            steps.items[b->y * w + b->x] += b->dir;

            if (action == '/')
            {
                if (b->dir == RIGHT)
                {
                    b->dir = UP;
                }
                else if (b->dir == UP)
                {
                    b->dir = RIGHT;
                }
                else if (b->dir == BOTTOM)
                {
                    b->dir = LEFT;
                }
                else if (b->dir == LEFT)
                {
                    b->dir = BOTTOM;
                }
            }
            else if (action == '\\')
            {
                if (b->dir == RIGHT)
                {
                    b->dir = BOTTOM;
                }
                else if (b->dir == UP)
                {
                    b->dir = LEFT;
                }
                else if (b->dir == BOTTOM)
                {
                    b->dir = RIGHT;
                }
                else if (b->dir == LEFT)
                {
                    b->dir = UP;
                }
            }
            else if (action == '|')
            {
                if (b->dir == RIGHT || b->dir == LEFT)
                {
                    b->dir = UP;
                    Beam n = {.x = b->x, .y = b->y, .dir = BOTTOM, .active = true};
                    AC_ARRAY_PUSH(beams, n);
                }
            }
            else if (action == '-')
            {
                if (b->dir == UP || b->dir == BOTTOM)
                {
                    b->dir = LEFT;
                    Beam n = {.x = b->x, .y = b->y, .dir = RIGHT, .active = true};
                    AC_ARRAY_PUSH(beams, n);
                }
            }

            switch (b->dir)
            {
            case RIGHT:
                b->x++;
                break;
            case LEFT:
                b->x--;
                break;
            case UP:
                b->y--;
                break;
            case BOTTOM:
                b->y++;
                break;
            }
            if (b->x >= w || b->y >= h)
            {
                b->active = false;
            }
        }
        // dump_steps(map, steps, h, w, beams);
    }

    size_t ret = 0;
    for (size_t i = 0; i < w * h; i++)
    {
        if (steps.items[i] > 0)
        {
            ret++;
        }
    }

    AC_ARRAY_DESTROY(steps);
    AC_ARRAY_DESTROY(beams);

    return ret;
}

size_t
part1(StringArray map)
{
    return solve(map, (Beam){.x = 0, .y = 0, .dir = RIGHT, .active = true});
}

size_t
part2(StringArray map)
{
    size_t w = map.items[0].count;
    size_t h = map.count;
    size_t total = 0;
    size_t tmp;

    for (size_t x = 0; x < w -1; x++)
    {
        tmp = solve(map, (Beam){.x = x, .y = 0, .dir = BOTTOM, .active = true});
        total = max(tmp, total);
        tmp = solve(map, (Beam){.x = x, .y = h-1, .dir = UP, .active = true});
        total = max(tmp, total);
    }
    for (size_t y = 0; y < h -1; y++)
    {
        tmp = solve(map, (Beam){.x = 0, .y = y, .dir = RIGHT, .active = true});
        total = max(tmp, total);
        tmp = solve(map, (Beam){.x = w-1, .y = h-1, .dir = LEFT, .active = true});
        total = max(tmp, total);
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
