cmake_minimum_required(VERSION 3.15)
project(SlidingBlocksPuzzleProceduralSolution)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(SlidingBlocksPuzzleProceduralSolution NSlidingPuzzle.cpp)

target_compile_options(SlidingBlocksPuzzleProceduralSolution PRIVATE
    -O3 -march=native -funroll-loops -fomit-frame-pointer
)
