#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MASK_NORTH 1
#define MASK_EAST 2
#define MASK_SOUTH 4
#define MASK_WEST 8

// typedef enum
// {
//     GROUND = 0,                   // .
//     EW = MASK_EAST | MASK_WEST,   // -   (\u196) C4
//     NS = MASK_NORTH | MASK_SOUTH, // |   (\u179) B3
//     SW = MASK_SOUTH | MASK_WEST,  // 7   (\u191) BF
//     SE = MASK_SOUTH | MASK_EAST,  // F   (\u218) DA
//     NW = MASK_NORTH | MASK_WEST,  // J   (\u217) D9
//     NE = MASK_NORTH | MASK_EAST   // L   (\u192) C0
// } TileType;

typedef struct tile_info_struct
{
    char id;
    size_t dir1;
    size_t dir2;
} TileInfo;

TileInfo tiles[] = {
    {.id = '-', .dir1 = MASK_EAST, .dir2 = MASK_WEST},   // ━
    {.id = '|', .dir1 = MASK_NORTH, .dir2 = MASK_SOUTH}, // ┃
    {.id = '7', .dir1 = MASK_SOUTH, .dir2 = MASK_WEST},  // ┓
    {.id = 'F', .dir1 = MASK_SOUTH, .dir2 = MASK_EAST},  // ┏
    {.id = 'J', .dir1 = MASK_NORTH, .dir2 = MASK_WEST},  // ┛
    {.id = 'L', .dir1 = MASK_NORTH, .dir2 = MASK_EAST},  // ┗
};
size_t num_tiles = 6;

typedef struct grid_pos_struct
{
    size_t x;
    size_t y;
} GridPos;

size_t
get_oposited_direction(size_t dir)
{
    size_t ret = 0;
    switch (dir)
    {
    case MASK_NORTH:
        ret = MASK_SOUTH;
        break;
    case MASK_EAST:
        ret = MASK_WEST;
        break;
    case MASK_SOUTH:
        ret = MASK_NORTH;
        break;
    case MASK_WEST:
        ret = MASK_EAST;
        break;
    }
    return ret;
}

GridPos
move_grid_pos(GridPos pos, size_t dir)
{
    int dx = (dir == MASK_EAST) ? 1 : ((dir == MASK_WEST) ? -1 : 0);
    int dy = (dir == MASK_SOUTH) ? 1 : ((dir == MASK_NORTH) ? -1 : 0);
    pos.y += dy;
    pos.x += dx;
    return pos;
}

size_t
get_directions(StringArray map, GridPos pos)
{
    size_t ret = 0;
    char current = map.items[pos.y].items[pos.x];
    for (size_t i = 0; i < num_tiles; i++)
    {
        if (current == tiles[i].id)
        {
            ret = tiles[i].dir1 | tiles[i].dir2;
            break;
        }
    }
    return ret;
}

bool can_move(StringArray map, GridPos pos, size_t dir)
{
    int maxy = map.count;
    int maxx = map.items[0].count;

    GridPos p = move_grid_pos(pos, dir);
    if (p.y >= maxy || p.x >= maxx)
    {
        return false;
    }

    size_t op_dir = get_oposited_direction(dir);
    return (get_directions(map, p) & op_dir) != 0;
}

size_t
get_start_tile_idx(StringArray map, GridPos pos)
{
    size_t ret = STRING_NPOS;
    char current = map.items[pos.y].items[pos.x];
    if (current != 'S')
    {
        return ret;
    }

    size_t dirs = 0;
    if (can_move(map, pos, MASK_NORTH))
        dirs |= MASK_NORTH;
    if (can_move(map, pos, MASK_EAST))
        dirs |= MASK_EAST;
    if (can_move(map, pos, MASK_SOUTH))
        dirs |= MASK_SOUTH;
    if (can_move(map, pos, MASK_WEST))
        dirs |= MASK_WEST;

    for (size_t i = 0; i < num_tiles; i++)
    {
        if (dirs == (tiles[i].dir1 | tiles[i].dir2))
        {
            ret = i;
            break;
        }
    }
    return ret;
}

bool next_move(StringArray map, GridPos *pos, size_t *dir)
{
    GridPos p = move_grid_pos(*pos, *dir);
    size_t op_dir = get_oposited_direction(*dir);
    size_t dirs = get_directions(map, p);
    bool found = (dirs != 0);
    if (found)
    {
        *dir = dirs ^ op_dir;
        *pos = p;
    }
    return found;
}

GridPos
find_start(StringArray map)
{
    GridPos pos = {0};
    for (size_t y = 0; y < map.count; y++)
    {
        for (size_t x = 0; x < map.items[y].count; x++)
        {
            if (map.items[y].items[x] == 'S')
            {
                pos.y = y;
                pos.x = x;
                return pos;
            }
        }
    }
}

void dump_map(StringArray map)
{
    for (size_t y = 0; y < map.count; y++)
    {
        for (size_t x = 0; x < map.items[y].count; x++)
        {
            printf("%c", map.items[y].items[x]);
        }
        printf("\n");
    }
}

char translate_symbol(char symbol)
{
    if (symbol == '-')
    {
        symbol = '=';
    }
    else if (symbol == '|')
    {
        symbol = '!';
    }
    else if (symbol == '7')
    {
        symbol = ')';
    }
    else if (symbol == 'F')
    {
        symbol = '(';
    }
    else if (symbol == 'J')
    {
        symbol = ']';
    }
    else if (symbol == 'L')
    {
        symbol = '[';
    }

    return symbol;
}

size_t 
solve(StringArray map, GridPos start)
{
    printf("[INFO] Start at this point (%lu, %lu)\n", start.x, start.y);
    size_t idx = get_start_tile_idx(map, start);
    if (idx == STRING_NPOS)
    {
        printf("[ERROR] No start found.\n");
    }
    size_t dir = tiles[idx].dir1;
    GridPos pos = start;

    size_t steps = 0;
    bool cont = true;
    while (cont)
    {
        steps++;
        cont &= next_move(map, &pos, &dir);
        map.items[pos.y].items[pos.x] = translate_symbol(map.items[pos.y].items[pos.x]);
    }
    printf("[INFO] End at this point (%lu, %lu)\n", pos.x, pos.y);
    return steps;
}

size_t
count_points_inside(StringArray map)
{
    size_t total = 0;
    for (size_t y = 0; y < map.count; y++)
    {
        size_t line_count = 0;
        size_t edges = 0;
        size_t open_south = 0;
        size_t open_north = 0;
        for (size_t x = 0; x < map.items[y].count; x++)
        {
            char current = map.items[y].items[x];

            if (current == '#' || current == 'S' || current == '=' || current == '!' || current == ')' || current == '(' || current == ']' || current == '[')
            {
                if (current == '!')
                {
                    edges++;
                }
                else if (current == '(' || current == ')')
                {
                    if (open_south > 0)
                    {
                        edges++;
                        open_south--;
                    }
                    else
                    {
                        open_north++;
                    }
                }
                else if ((current == '[' || current == ']'))
                {
                    if (open_north > 0)
                    {
                        edges++;
                        open_north--;
                    }
                    else
                    {
                        open_south++;
                    }
                }
                // ignore '='
            }
            else
            {
                if (((edges % 2) != 0))
                {
                    line_count += 1;
                    current = 'I';
                }
                else
                {
                    current = 'O';
                }
            }
            map.items[y].items[x] = current;
        }
        total += line_count;
    }
    return total;
}

size_t
part1(StringArray lines)
{
    GridPos start = find_start(lines);
    return solve(lines, start) / 2;
}

size_t
part2(StringArray lines)
{
    return count_points_inside(lines);
}

int main(int argc, char **argv)
{
    CharArray content = read_file_content("assets/input01.txt");
    StringArray lines = string_split(content, '\n');

    size_t p1 = part1(lines);
    printf(" Part 1 total:  %lu\n", p1);

    //dump_map(lines);

    size_t p2 = part2(lines);
    printf(" Part 2 total:  %lu\n", p2);
    
    //dump_map(lines);

    AC_ARRAY_DESTROY(lines);
    AC_ARRAY_DESTROY(content);
    return 0;
}
