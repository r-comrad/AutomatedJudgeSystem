cmake_minimum_required(VERSION 3.21)
project(ChineseTester)

set(CMAKE_CXX_STANDARD 14)

#file (GLOB ChineseTester  CONFIGURE_DEPENDS "*.h" "*.cpp")

#add_executable(ChineseTester source/main.cpp)
add_executable(ChineseTester
        source/main.cpp
        source/process.cpp
        source/core.cpp
        source/database.cpp
        source/database_query.cpp
        source/domaine.cpp
        source/sqlite3.c
        source/submission_information.cpp
        source/testlib_message.cpp
        )
