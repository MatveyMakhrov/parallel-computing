cmake_minimum_required(VERSION 3.15)
project(SobelLab7 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определяем компилятор и устанавливаем соответствующие флаги
if(MSVC)
    # Для Visual Studio
    add_compile_options(/arch:AVX2 /O2 /Oi /std:c++17)
    add_compile_options(/EHsc)  # Включаем исключения C++
else()
    # Для GCC/Clang (Linux/macOS/MinGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mfma -O3")
endif()

add_executable(sobel_lab7
    src/main.cpp
    src/image.cpp
    src/sobel_filter.cpp
)

# Для Linux/macOS нужно линковать с математической библиотекой
if(NOT MSVC)
    target_link_libraries(sobel_lab7 m)
endif()