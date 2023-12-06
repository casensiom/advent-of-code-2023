#include "common.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

int
main(int argc, char **argv) {
    //--
    const char *RAW_DATA = "The quick brown fox jumps over the lazy dog. 1234567890";
    size_t      LEN_DATA = strlen(RAW_DATA);

    String test_data = string_create(RAW_DATA);
    assert(test_data.items == RAW_DATA);
    assert(test_data.count == LEN_DATA);

    String test_data2 = string_create_with_len(RAW_DATA, LEN_DATA);
    assert(test_data2.items == RAW_DATA);
    assert(test_data2.count == LEN_DATA);

    String find_data;
    size_t pos;
    find_data = string_create("lazy dog");
    pos       = string_find(test_data, find_data);
    assert(pos == 35);

    pos = string_find(find_data, test_data);
    assert(pos == STRING_NPOS);

    find_data = string_create("god yzal");
    pos       = string_find(test_data, find_data);
    assert(pos == STRING_NPOS);
    printf("[ PASS ] creation.\n");

    //--
    bool   equals;
    String equals_data = string_create("lazy dog");
    assert(string_equals(string_create("lazy dog"), string_create("lazy dog")));
    assert(string_equals(string_create("lazy dog"), string_create("lazy dog")));
    assert(string_equals_case(string_create("lazy dog"), string_create("lazy dog")));
    assert(!string_equals_case(string_create("lazy dog"), string_create("LAZY DOG")));

    equals_data = string_create("The quick");
    equals      = string_starts_with(test_data, equals_data);
    assert(equals == true);

    equals_data = string_create("the quick");
    equals      = string_starts_with(test_data, equals_data);
    assert(equals == false);

    equals_data = string_create("00000");
    equals      = string_starts_with(test_data, equals_data);
    assert(equals == false);

    equals_data = string_create("dog. 1234567890");
    equals      = string_ends_with(test_data, equals_data);
    assert(equals == true);

    equals_data = string_create("Dog. 1234567890");
    equals      = string_ends_with(test_data, equals_data);
    assert(equals == false);

    equals_data = string_create("00000");
    equals      = string_ends_with(test_data, equals_data);
    assert(equals == false);

    printf("[ PASS ] comparison.\n");

    //--
    String trim_data = string_create("  \tdata  \t");
    String ltrim_res = string_ltrim(trim_data);
    assert(string_equals(ltrim_res, string_create("data  \t")));

    String rtrim_res = string_rtrim(trim_data);
    assert(string_equals(rtrim_res, string_create("  \tdata")));

    String trim_res = string_trim(trim_data);
    assert(string_equals(trim_res, string_create("data")));

    printf("[ PASS ] trim.\n");

    //--
    String remove_str = string_clone("  1 23  45    56");
    size_t removed = string_remove(&remove_str, ' ');
    assert(string_equals(remove_str, string_create("1234556")));
    AC_ARRAY_DESTROY(remove_str);

    printf("[ PASS ] remove.\n");

    //--
    StringArray parts = string_split(string_create("1 2 3 4 5 6  7"), ' ');

    String joined_with_chr = string_join_with_char(parts, ':');
    assert(string_equals(joined_with_chr, string_create("1:2:3:4:5:6:7")));
    AC_ARRAY_DESTROY(joined_with_chr);

    String joined_with_str = string_join_with_string(parts, string_create(":-:"));
    assert(string_equals(joined_with_str, string_create("1:-:2:-:3:-:4:-:5:-:6:-:7")));
    AC_ARRAY_DESTROY(joined_with_str);

    String joined = string_join(parts);
    assert(string_equals(joined, string_create("1234567")));
    AC_ARRAY_DESTROY(joined);

    AC_ARRAY_DESTROY(parts);
    
    printf("[ PASS ] split and join.\n");



    return 0;
}