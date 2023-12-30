#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum
{
    LESS_THAN = 0,
    BIGGEST_THAN
} Operator;

typedef enum
{
    CAT_X,
    CAT_M,
    CAT_A,
    CAT_S,
    NUM_CATEGORIES
} Category;

typedef struct range_struct
{
    size_t vmin;
    size_t vmax;
} Range;


typedef struct condition_struct
{
    String name;
    Operator operator;
    size_t value;
    String target;
    bool present;
} Condition;
AC_ARRAY_DEFINE(Condition);
AC_ARRAY_DEFINE(ConditionArray);

typedef struct instruction_struct
{
    String label;
    ConditionArray conditions;
    String default_target;
} Instruction;
AC_ARRAY_DEFINE(Instruction);

typedef struct data_struct
{
    size_t data[NUM_CATEGORIES];
} Input;
AC_ARRAY_DEFINE(Input);

static ConditionArrayArray range_conditions = {0};
static InstructionArray instructions = {0};
static InputArray inputs = {0};


size_t
get_category(String name) {
    if(name.items[0] == 'x') {
        return CAT_X;
    }
    else if(name.items[0] == 'm') {
        return CAT_M;
    }
    else if(name.items[0] == 'a') {
        return CAT_A;
    }
    else if(name.items[0] == 's') {
        return CAT_S;
    }
    return STRING_NPOS;
}

void parser(StringArray lines)
{
    size_t i = 0;
    while (lines.items[i].count > 0)
    {
        String l = lines.items[i];
        Instruction tmp = {0};
        StringArray parts = string_split(l, '{');
        tmp.label = parts.items[0];
        tmp.conditions = AC_ARRAY_CREATE(Condition, 4);
        StringArray conds = string_split(parts.items[1], ',');
        for (size_t i = 0; i < conds.count - 1; i++)
        {
            String c = conds.items[i];
            Condition condition = {0};
            condition.name = parser_consume_chars(&c, 1);
            String op = parser_consume_chars(&c, 1);
            condition.operator=(op.items[0] == '<') ? LESS_THAN : BIGGEST_THAN;
            condition.value = parser_consume_int(&c, NULL);
            parser_consume_chars(&c, 1);
            condition.target = parser_consume_literal(&c);
            condition.present = true;

            AC_ARRAY_INSERT(tmp.conditions, condition);
        }
        tmp.default_target = conds.items[conds.count - 1];
        tmp.default_target.count--;
        AC_ARRAY_PUSH(instructions, tmp);

        AC_ARRAY_DESTROY(conds);
        AC_ARRAY_DESTROY(parts);
        i++;
    }
    i++;

    while (i < lines.count)
    {
        String l = lines.items[i];
        parser_consume_chars(&l, 1);
        StringArray vars = string_split(l, ',');

        Input tmp;
        for (size_t i = 0; i < vars.count; i++)
        {
            String l = vars.items[i];
            String name = parser_consume_chars(&l, 1);
            parser_consume_chars(&l, 1);
            size_t value = parser_consume_int(&l, NULL);
            
            size_t idx =  get_category(name);
            tmp.data[idx] = value;
        }
        AC_ARRAY_PUSH(inputs, tmp);

        AC_ARRAY_DESTROY(vars);
        i++;
    }
}

void dump()
{
    printf("Instructions:\n");
    for (size_t i = 0; i < instructions.count; i++)
    {
        Instruction tmp = instructions.items[i];
        printf(" - " STRING_FMT " -> " STRING_FMT ": [", STRING_ARG(tmp.label), STRING_ARG(tmp.default_target));
        for (size_t j = 0; j < tmp.conditions.count; j++)
        {
            Condition cond = tmp.conditions.items[j];
            if(cond.present) {
                printf("(" STRING_FMT " %c %lu -> " STRING_FMT "), ", STRING_ARG(cond.name), (cond.operator== LESS_THAN) ? '<' : '>', cond.value, STRING_ARG(cond.target));
            }
        }
        printf("]\n");
    }

    printf("Inputs:\n");
    const char *names = "xmas";
    for (size_t i = 0; i < inputs.count; i++)
    {
        Input tmp = inputs.items[i];
        printf(" - [");
        for (size_t j = 0; j < NUM_CATEGORIES; j++)
        {
            printf("%c=%lu, ", names[j], tmp.data[j]);
        }
        printf("]\n");
    }

    printf("Ranges:\n");
    for (size_t i = 0; i < range_conditions.count; i++)
    {
        printf("%lu) [", i);
        ConditionArray tmp = range_conditions.items[i];
        for (size_t j = 0; j < tmp.count; j++)
        {
            Condition cond = tmp.items[j];
            printf("(" STRING_FMT " %c %lu), ", STRING_ARG(cond.name), (cond.operator== LESS_THAN) ? '<' : '>', cond.value);
        }
        printf("]\n");
    }
}

void destroy()
{
    for (size_t i = 0; i < instructions.count; i++)
    {
        AC_ARRAY_DESTROY(instructions.items[i].conditions);
    }
    AC_ARRAY_DESTROY(instructions);

    AC_ARRAY_DESTROY(inputs);

    for (size_t i = 0; i < range_conditions.count; i++)
    {
        AC_ARRAY_DESTROY(range_conditions.items[i]);
    }
    AC_ARRAY_DESTROY(range_conditions);
}

Condition
reverse_condition(Condition cond)
{
    if (cond.operator== LESS_THAN)
    {
        cond.operator= BIGGEST_THAN;
        cond.value--;
    }
    else if (cond.operator== BIGGEST_THAN)
    {
        cond.operator= LESS_THAN;
        cond.value++;
    }
    return cond;
}

void fill_ranges()
{
    String accepted = string_create("A");
    String rejected = string_create("R");
    InstructionArray queue = AC_ARRAY_CREATE(Instruction, instructions.count);

    Instruction init = (Instruction){.label = string_create("in"), .conditions = {0}};
    AC_ARRAY_INSERT(queue, init);

    while (queue.count > 0)
    {
        Instruction current;
        AC_ARRAY_POP_FRONT(queue, current);

        // 1) Check finished
        if (string_equals(current.label, accepted))
        {
            // Store all conditions!
            AC_ARRAY_PUSH(range_conditions, current.conditions);
            continue;
        }
        if (string_equals(current.label, rejected))
        {
            AC_ARRAY_DESTROY(current.conditions);
            continue;
        }

        // 2) locate current label on instructions
        Instruction inst = {0};
        for (size_t j = 0; j < instructions.count; j++)
        {
            Instruction found_inst = instructions.items[j];
            if (string_equals(current.label, found_inst.label))
            {
                inst = found_inst;
                break;
            }
        }

        Instruction default_case = {
            .label = inst.default_target,
            .conditions = {0},
            .default_target = {0}};
        AC_ARRAY_CLONE(Condition, current.conditions, default_case.conditions);

        // 3) Check conditions
        for (size_t c = 0; c < inst.conditions.count; c++)
        {
            Condition cond = inst.conditions.items[c];

            Instruction clone = {
                .label = cond.target,
                .conditions = {0},
                .default_target = {0}};
            AC_ARRAY_CLONE(Condition, current.conditions, clone.conditions);
            AC_ARRAY_PUSH(clone.conditions, cond);
            AC_ARRAY_PUSH(queue, clone);

            // store reverse conditions for default case
            Condition reverse = reverse_condition(cond);
            AC_ARRAY_PUSH(default_case.conditions, reverse);
            AC_ARRAY_PUSH(current.conditions, reverse);
        }

        // enqueue rev_conds and target
        AC_ARRAY_PUSH(queue, default_case);

        AC_ARRAY_DESTROY(current.conditions);
    }
    AC_ARRAY_DESTROY(queue);
}


size_t
part1(StringArray lines)
{
    size_t total = 0;
    for (size_t i = 0; i < inputs.count; i++)
    {
        Input current = inputs.items[i];
    
        for (size_t i = 0; i < range_conditions.count; i++)
        {
            Range range[NUM_CATEGORIES] = {{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}};
            
            ConditionArray tmp = range_conditions.items[i];
            for (size_t j = 0; j < tmp.count; j++)
            {
                Condition cond = tmp.items[j];
                size_t idx = get_category(cond.name);
                if (cond.operator== LESS_THAN)
                {
                    range[idx].vmax = min(cond.value - 1, range[idx].vmax);
                }
                else
                {
                    range[idx].vmin = max(cond.value + 1, range[idx].vmin);
                }
            }

            size_t sum = 0;
            for (size_t c = 0; c < NUM_CATEGORIES; c++)
            {
                sum += current.data[c];
                if(current.data[c] < range[c].vmin ||  current.data[c] > range[c].vmax) {
                    sum = 0;
                    break;
                }
            }

            total += sum;
        }
    }
    return total;
}

size_t
part2(StringArray lines)
{

    size_t total = 0;
    for (size_t i = 0; i < range_conditions.count; i++)
    {
        Range range[NUM_CATEGORIES] = {{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}};
        
        ConditionArray tmp = range_conditions.items[i];
        for (size_t j = 0; j < tmp.count; j++)
        {
            Condition cond = tmp.items[j];
            size_t idx = get_category(cond.name);
            if (cond.operator== LESS_THAN)
            {
                range[idx].vmax = min(cond.value - 1, range[idx].vmax);
            }
            else
            {
                range[idx].vmin = max(cond.value + 1, range[idx].vmin);
            }
        }
        size_t product = 1;
        product *= (range[CAT_S].vmax - range[CAT_S].vmin) + 1;
        product *= (range[CAT_A].vmax - range[CAT_A].vmin) + 1;
        product *= (range[CAT_X].vmax - range[CAT_X].vmin) + 1;
        product *= (range[CAT_M].vmax - range[CAT_M].vmin) + 1;
        total += product;
    }

    return total;
}

int main(int argc, char **argv)
{
    CharArray content = read_file_content("assets/input01.txt");
    StringArray lines = string_split_ex(content, '\n', true);

    // Build data from text
    instructions = AC_ARRAY_CREATE(Instruction, lines.count);
    inputs = AC_ARRAY_CREATE(Input, lines.count);
    range_conditions = AC_ARRAY_CREATE(ConditionArray, instructions.count);
    parser(lines);
    fill_ranges();

    size_t p1 = part1(lines);
    printf(" Part 1 total:  %lu\n", p1);

    size_t p2 = part2(lines);
    printf(" Part 2 total:  %lu\n", p2);

    // destroy data generated
    destroy();

    AC_ARRAY_DESTROY(lines);
    AC_ARRAY_DESTROY(content);
    return 0;
}
