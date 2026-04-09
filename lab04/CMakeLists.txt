cmake_minimum_required(VERSION 3.10)

project(lab04)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/memory_tests.cpp
    main.cpp
)

add_executable(lab04 ${SOURCES})