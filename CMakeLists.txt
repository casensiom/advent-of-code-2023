cmake_minimum_required(VERSION 3.16)

project(advent-code-2023)
set(CMAKE_C_STANDARD 99)

enable_testing()
add_subdirectory(common)

foreach(DAY_NUM RANGE 1 31)
    if(${DAY_NUM} LESS 10)
        set(DAY_NUM "0${DAY_NUM}")
    endif()
    set(DAY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/day${DAY_NUM})
    if(EXISTS ${DAY_PATH})
        add_subdirectory(${DAY_PATH})
    endif()
endforeach()
