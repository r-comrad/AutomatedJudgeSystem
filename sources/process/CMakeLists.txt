cmake_minimum_required(VERSION 3.21)

add_library(
    process

    file_process.hpp
    file_process.cpp

    my_process.hpp
    my_process.cpp

    pipe_process.hpp
    pipe_process.cpp
)

