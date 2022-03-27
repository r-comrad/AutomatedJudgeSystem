cmake_minimum_required(VERSION 3.21)

add_library(
    database

    database_query.hpp
    database_query.cpp

    sqlite_database.hpp
    sqlite_database.cpp
)

