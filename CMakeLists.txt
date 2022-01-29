cmake_minimum_required(VERSION 3.21)
project(ChineseTester)
set(CMAKE_DEBUG_POSTFIX d)

add_library(tutorial_compiler_flags INTERFACE)
#add_subdirectory(sources)
#add_subdirectory(headers)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--no-as-needed -ldl -D_DEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--no-as-needed -ldl")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG")
OPTION(CHINESE_DEBUG ON)


set(EXECUTABLE_OUTPUT_PATH "../")

#file (GLOB ChineseTester  CONFIGURE_DEPENDS "*.h" "*.cpp")
include_directories(ChineseTester headers)
include_directories(ChineseTesters sources)
#add_executable(ChineseTester source/main.cpp)

#target_link_libraries(ChineseTester LINK_PUBLIC sqlite3)
add_compile_options(-l sqlite3)



add_executable(ChineseTester
        sources/core.cpp
        sources/database.cpp
        sources/database_query.cpp
        sources/domaine.cpp
        sources/file_process.cpp
        sources/main.cpp
        sources/my_process.cpp
        sources/pipe_process.cpp
        sources/problem_information.cpp
        sources/sqlite3.c
        sources/submission_information.cpp
        sources/testlib_message.cpp

        headers/core.h
        headers/database.h
        headers/database_query.h
        headers/domain.h
        headers/file_process.h
        headers/my_process.h
        headers/pipe_process.h
        headers/problem_information.h
        headers/submission_information.h
        headers/testlib_message.h
        )

#set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "ChineseTester_R_NewVegasEdition_V0")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "ChineseTester_D_NewVegasEdition_V0")
set_target_properties(${PROJECT_NAME}  PROPERTIES SUFFIX ".81")