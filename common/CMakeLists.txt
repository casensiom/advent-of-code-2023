message("Build common start")
add_library(common array.h str.h common.h common.c)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)