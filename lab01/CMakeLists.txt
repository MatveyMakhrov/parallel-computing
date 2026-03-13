cmake_minimum_required(VERSION 3.16)

project(lab01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(lab01
    src/main.cpp
    src/matrix.cpp
    src/timer.cpp
    src/statistics.cpp
)

target_include_directories(lab01 PRIVATE include)

if (WIN32)
    target_compile_definitions(lab01 PRIVATE NOMINMAX)
endif()