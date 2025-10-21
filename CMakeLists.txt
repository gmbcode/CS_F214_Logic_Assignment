cmake_minimum_required(VERSION 3.31)
project(Logic_Assignment)

set(CMAKE_CXX_STANDARD 20)

add_executable(Logic_Assignment main.cpp Q1.cpp Q1.h
        Q2.h
        Q2.cpp
        Q3.cpp
        Q3.h
        Q4.cpp
        Q4.h
        Q5.cpp
        Q5.h
        Q6.h
        Q6.cpp
        Q7.cpp
        Q7.h
        FileUtility.cpp
        FileUtility.h)

# Increase stack size (temporary testing) later maybe use balanced binary tree
if(MSVC)
    # MSVC compiler
    target_link_options(Logic_Assignment PRIVATE /STACK:16777216)
elseif(MINGW OR CMAKE_COMPILER_IS_GNUCXX)
    # MinGW or GCC: Set stack size to 16 MB
    target_link_options(Logic_Assignment PRIVATE -Wl,--stack,16777216)
elseif(APPLE)
    # macOS
    target_link_options(Logic_Assignment PRIVATE -Wl,-stack_size,0x1000000)
elseif(UNIX)
    # Linux
    target_link_options(Logic_Assignment PRIVATE -Wl,-z,stack-size=16777216)
endif()