cmake_minimum_required(VERSION 3.0)
project(ExternalSort)

cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0074 NEW)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

add_executable(ExternalSort
    ./src/main.cpp
    ./include/tape.h
    ./src/tape.cpp
    ./include/externalSort.h
    ./src/externalSort.cpp
)

target_include_directories(ExternalSort PUBLIC ./include)

add_executable(ExternalSortTest
    ./test/test.cpp
    ./include/tape.h
    ./src/tape.cpp
    ./include/externalSort.h
    ./src/externalSort.cpp
)

target_include_directories(ExternalSortTest PUBLIC ./include)

add_test(NAME ExternalSortTest COMMAND ExternalSortTest)
