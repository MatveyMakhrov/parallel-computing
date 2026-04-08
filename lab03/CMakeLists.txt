cmake_minimum_required(VERSION 3.10)

project(CPUID_Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpuid_app
    src/main.cpp
    src/cpuinfo.cpp
)