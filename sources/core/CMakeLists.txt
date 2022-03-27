cmake_minimum_required(VERSION 3.21)

add_library(
    core

    compiler.hpp
    compiler.cpp

    core.hpp
    core.cpp
)

#target_compile_definitions(CORE_VERSION PUBLIC )           