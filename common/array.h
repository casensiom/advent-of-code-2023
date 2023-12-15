#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdlib.h>
#include <string.h>

#define AC_ARRAY_DEFINE(TYPE)          \
    typedef struct TYPE##_struct_array \
    {                                  \
        TYPE *items;                   \
        size_t count;                  \
        size_t capacity;               \
    } TYPE##Array;

// Reserves memory AC_ARRAY_DESTROY must be invoked.
#define AC_ARRAY_CREATE(TYPE, CAPACITY)                                                        \
    (TYPE##Array)                                                                              \
    {                                                                                          \
        .items = (TYPE *)malloc((CAPACITY) * sizeof(TYPE)), .count = 0, .capacity = (CAPACITY) \
    }

// Doesn't reserve memory, it just points to another pointer. Keeps capacity to zero.
#define AC_ARRAY_SET(TYPE, VALUE, COUNT)                          \
    (TYPE##Array)                                                 \
    {                                                             \
        .items = (TYPE *)(VALUE), .count = (COUNT), .capacity = 0 \
    }

#define AC_ARRAY_INSERT(INSTANCE, ITEM)                  \
    do                                                   \
    {                                                    \
        if ((INSTANCE).capacity > (INSTANCE).count)      \
        {                                                \
            (INSTANCE).items[(INSTANCE).count] = (ITEM); \
            (INSTANCE).count++;                          \
        }                                                \
    } while (0)

#define AC_ARRAY_PUSH(INSTANCE, ITEM)                                                         \
    do                                                                                        \
    {                                                                                         \
        if ((INSTANCE).capacity <= (INSTANCE).count)                                          \
        {                                                                                     \
            if ((INSTANCE).capacity == 0)                                                     \
            {                                                                                 \
                (INSTANCE).capacity == 1;                                                     \
            }                                                                                 \
            (INSTANCE).capacity *= 2;                                                         \
            (INSTANCE).items = realloc((INSTANCE).items, (INSTANCE).capacity * sizeof(ITEM)); \
        }                                                                                     \
        AC_ARRAY_INSERT(INSTANCE, ITEM);                                                      \
    } while (0)

#define AC_ARRAY_POP(INSTANCE, ITEM)                       \
    do                                                     \
    {                                                      \
        if ((INSTANCE).count > 0)                          \
        {                                                  \
            ITEM = (INSTANCE).items[(INSTANCE).count - 1]; \
            (INSTANCE).count--;                            \
        }                                                  \
    } while (0)

#define AC_ARRAY_SLICE(INSTANCE, NUM, COPY)          \
    do                                               \
    {                                                \
        (COPY).capacity = 0;                         \
        if ((NUM) > 0)                               \
        {                                            \
            (COPY).items = (INSTANCE).items + (NUM); \
            (COPY).count = (INSTANCE).count - (NUM); \
        }                                            \
        else                                         \
        {                                            \
            (COPY).items = (INSTANCE).items;         \
            (COPY).count = (INSTANCE).count + (NUM); \
        }                                            \
    } while (0)

#define AC_ARRAY_CLEAN(INSTANCE)                                                        \
    do                                                                                  \
    {                                                                                   \
        memset((INSTANCE).items, 0, sizeof((INSTANCE).items[0]) * (INSTANCE).capacity); \
        (INSTANCE).count = 0;                                                           \
    } while (0)

#define AC_ARRAY_REMOVE(INSTANCE, POS)                                                               \
    do                                                                                               \
    {                                                                                                \
        for (size_t array_iterator = (POS); array_iterator < (INSTANCE).count - 1; array_iterator++) \
        {                                                                                            \
            (INSTANCE).items[array_iterator] = (INSTANCE).items[array_iterator + 1];                 \
        }                                                                                            \
        (INSTANCE).count--;                                                                          \
    } while (0)

#define AC_ARRAY_DESTROY(INSTANCE)                               \
    do                                                           \
    {                                                            \
        if ((INSTANCE).items != NULL && (INSTANCE).capacity > 0) \
            free((INSTANCE).items);                              \
        (INSTANCE).items = NULL;                                 \
        (INSTANCE).count = 0;                                    \
        (INSTANCE).capacity = 0;                                 \
    } while (0)

#endif // _ARRAY_H_