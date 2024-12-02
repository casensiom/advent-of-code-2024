cmake_minimum_required(VERSION 3.16)

project(advent-code-2024)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_subdirectory(toolit)

foreach(DAY_NUM RANGE 1 31)
    if(${DAY_NUM} LESS 10)
        set(DAY_NUM "0${DAY_NUM}")
    endif()
    set(DAY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/day${DAY_NUM})
    if(EXISTS ${DAY_PATH})
        add_subdirectory(${DAY_PATH})
    endif()
endforeach()