cmake_minimum_required(VERSION 3.24)
project(Allocator)

set(CMAKE_CXX_STANDARD 23)

include_directories(Allocator tests)

add_executable(Allocator main.cpp
        Allocator/Pool.h
        Allocator/MemoryPoolsAllocator.h
        Allocator/ASettings.h
        Allocator/PoolSettings.h
        tests/Test.h tests/Test.cpp
        )
