cmake_minimum_required(VERSION 3.21)

add_library(
    SQLite

    shell.c
    sqlite3.c

    sqlite3.h
    sqlite3ext.h
)
