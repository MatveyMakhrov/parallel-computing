cmake_minimum_required(VERSION 3.10)

project(VectorOpsCPU)

set(CMAKE_CXX_STANDARD 17)

add_executable(main
    main.cpp
    src/vec_ops.cpp
)