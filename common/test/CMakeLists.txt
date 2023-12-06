message("Build common-test start")
add_executable(common-test test_str.c)
target_link_libraries(common-test common)

add_test(NAME common-test COMMAND common-test)
