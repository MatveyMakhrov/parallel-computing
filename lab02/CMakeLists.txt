cmake_minimum_required(VERSION 3.16)

project(lab02 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release" OR NOT CMAKE_BUILD_TYPE)
    if (MSVC)
        add_compile_options(/O2 /Ob2 /Oi /Ot /Oy /GL)
    else()
        add_compile_options(-O3 -march=native -funroll-loops)
    endif()
endif()

add_executable(lab02
    src/main.cpp
    src/matrix_ops.cpp
)

target_include_directories(lab02 PRIVATE include)

if (WIN32)
    target_compile_definitions(lab02 PRIVATE NOMINMAX)
endif()

add_custom_command(TARGET lab02 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/results"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/plots"
    COMMENT "Creating results and plots directories"
)

set_target_properties(lab02 PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)