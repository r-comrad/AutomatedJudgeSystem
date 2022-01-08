cmake_minimum_required(VERSION 3.21)
project(ChineseTester)
#add_subdirectory(sources)
#add_subdirectory(headers)
set(CMAKE_CXX_STANDARD 14)

#file (GLOB ChineseTester  CONFIGURE_DEPENDS "*.h" "*.cpp")

#add_executable(ChineseTester source/main.cpp)
add_executable(ChineseTester
        ./sources/main.cpp
        ./sources/my_process.cpp
        ./sources/core.cpp
        ./sources/database.cpp
        ./sources/database_query.cpp
        ./sources/domaine.cpp
        ./sources/sqlite3.c
        ./sources/submission_information.cpp
        ./sources/testlib_message.cpp
        ./headers/core.h
        ./headers/database.h
        ./headers/database_query.h
        ./headers/domain.h
        ./headers/file_process.h
        ./headers/my_process.h
        ./headers/pipe_process.h
        ./headers/sqlite3.h
        ./headers/sqlite3ext.h
        ./headers/submission_information.h
        ./headers/testlib_message.h
        )