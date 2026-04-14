cmake_minimum_required(VERSION 3.16)
project(SIMD_Lab5)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mavx2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

include_directories(include)

add_executable(simd_lab
    src/main.cpp
    src/baseline.cpp
    # src/simd_scalar.cpp
    src/simd_vector.cpp
    src/utils.cpp
)