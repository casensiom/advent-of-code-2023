#include "common.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum
{
    HandType_Five,
    HandType_Four,
    HandType_Full,
    HandType_Three,
    HandType_TwoPair,
    HandType_OnePair,
    HandType_High,
    HandTypeCount
} HandType;

typedef struct hand_struct
{
    String cards;
    size_t bid;
    HandType type;
} Hand;
AC_ARRAY_DEFINE(Hand);

#define CardCount 13
char card_value[CardCount] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
int card_score[CardCount] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

int get_card_value(char val)
{
    for (size_t i = 0; i < CardCount; i++)
    {
        if (val == card_value[i])
        {
            return card_score[i];
        }
    }

    return 999;
}

String
get_hand_type_name(HandType type)
{
    String ret;
    switch (type)
    {
    case HandType_Five:
        ret = string_create("Five");
        break;
    case HandType_Four:
        ret = string_create("Four");
        break;
    case HandType_Full:
        ret = string_create("Full");
        break;
    case HandType_Three:
        ret = string_create("Three");
        break;
    case HandType_TwoPair:
        ret = string_create("TwoPair");
        break;
    case HandType_OnePair:
        ret = string_create("OnePair");
        break;
    case HandType_High:
        ret = string_create("High");
        break;
    default:
        break;
    }
    return ret;
}

HandType compute_hand_type(String cards, bool use_jokers)
{
    size_t count_j = 0;
    size_t max1 = 0;
    size_t max2 = 0;

    for (size_t i = 0; i < CardCount; i++)
    {
        char value = card_value[i];
        int t = get_card_value(value);
        size_t count = string_count(cards, value);
        if (use_jokers && value == 'J')
        {
            continue;
        }

        if(count >= max1) {
            if(max1 > max2) 
            {
                max2 = max1;
            }
            max1 = count;
        } else if(count > max2) {
            max2 = count;
        }
        // printf("'%c' -> %lu (%lu, %lu)\n", value, count, max1, max2);
    }

    if(max2 > max1) {
        size_t tmp = max2;
        max2 = max1;
        max1 = tmp;
    }

    if (use_jokers)
    {
        count_j = string_count(cards, 'J');
        max1 += count_j;
    }

    HandType ret = HandTypeCount;
    if(max1 == 5) {
        ret = HandType_Five;
    } else if(max1 == 4) {
        ret = HandType_Four;
    } else if(max1 == 3) {
        if(max2 == 2) {
            ret = HandType_Full;
        }else {
            ret = HandType_Three;
        }
    } else if(max1 == 2) {
        if(max2 == 2) {
            ret = HandType_TwoPair;
        } else {
            ret = HandType_OnePair;
        }
    } else {
        ret = HandType_High;
    }

    // printf(" - '" STRING_FMT "' [%lu, %lu] (" STRING_FMT ") %s\n", STRING_ARG(cards), max1, max2, STRING_ARG(get_hand_type_name(ret)), count_j > 0 ? "*" : " ");
    
    return ret;
}

int get_best_card(Hand a, Hand b)
{
    for (size_t i = 0; i < a.cards.count; i++)
    {
        int type_a = get_card_value(a.cards.items[i]);
        int type_b = get_card_value(b.cards.items[i]);
        if (type_a < type_b)
            return -1;
        if (type_a > type_b)
            return 1;
    }
    return 0;
}

int comp(const void *elem1, const void *elem2)
{
    Hand a = *((Hand *)elem1);
    Hand b = *((Hand *)elem2);

    if (a.type < b.type)
        return -1;
    if (a.type > b.type)
        return 1;
    return get_best_card(a, b);
}

size_t
part1(StringArray lines)
{
    HandArray hands = AC_ARRAY_CREATE(Hand, lines.count);
    for (size_t i = 0; i < lines.count; i++)
    {
        String line = lines.items[i];
        String hand = parser_consume_chars(&line, 5);
        parser_consume_chars(&line, 1);
        size_t bid = parser_consume_int(&line, NULL);

        AC_ARRAY_INSERT(hands, ((Hand){
                                   .cards = hand,
                                   .bid = bid,
                                   .type = compute_hand_type(hand, false)}));
    }

    qsort(hands.items, hands.count, sizeof(Hand), comp);
    size_t total = 0;
    for (size_t i = 0; i < hands.count; i++)
    {
        // printf("%lu) '" STRING_FMT "' %lu\n", i, STRING_ARG(hands.items[i].cards), hands.items[i].bid);
        total += (hands.count - i) * hands.items[i].bid;
    }

    AC_ARRAY_DESTROY(hands);

    return total;
}

size_t
part2(StringArray lines)
{
    HandArray hands = AC_ARRAY_CREATE(Hand, lines.count);
    for (size_t i = 0; i < lines.count; i++)
    {
        String line = lines.items[i];
        String hand = parser_consume_chars(&line, 5);
        parser_consume_chars(&line, 1);
        size_t bid = parser_consume_int(&line, NULL);

        AC_ARRAY_INSERT(hands, ((Hand){
                                   .cards = hand,
                                   .bid = bid,
                                   .type = compute_hand_type(hand, true)}));
    }

    card_score[3] = CardCount + 1;
    qsort(hands.items, hands.count, sizeof(Hand), comp);
    size_t total = 0;
    for (size_t i = 0; i < hands.count; i++)
    {
        //size_t jc = string_count(hands.items[i].cards, 'J');
        //printf("%lu) '" STRING_FMT "' %lu (" STRING_FMT ") %s\n", i, STRING_ARG(hands.items[i].cards), hands.items[i].bid, STRING_ARG(get_hand_type_name(hands.items[i].type)), jc > 0 ? "*" : " ");
        total += (hands.count - i) * hands.items[i].bid;
    }

    AC_ARRAY_DESTROY(hands);

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
