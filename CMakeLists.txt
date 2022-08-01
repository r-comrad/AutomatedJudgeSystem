cmake_minimum_required(VERSION 3.21.3)
project(ChineseTeste VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile options
if (MSVC_VERSION GREATER_EQUAL "1900")
    add_compile_options("/std:c++latest")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--no-as-needed -ldl")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

    set(PQXX /usr/include/pqxx)
    find_library(PQXX_LIB pqxx)
    find_library(PQ_LIB pq)
endif()

# Boost
#ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")
#find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

#  Code include
include(${CMAKE_CURRENT_SOURCE_DIR}/sources/sources.cmake)
get_sources(${CMAKE_CURRENT_SOURCE_DIR}/sources SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sources)

#  Libraries 
add_subdirectory(libraries)
if (MSVC_VERSION GREATER_EQUAL "1900")
    target_compile_definitions(${PROJECT_NAME} PRIVATE SQLITE)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE POSTGRESQL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PQXX_LIB} ${PQ_LIB})
endif()

#  Compiler defenitions
if (MSVC_VERSION GREATER_EQUAL "1900")
    target_compile_definitions(${PROJECT_NAME} PRIVATE BILL_WINDOWS)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE LINUS_LINUX)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PQXX_LIB} ${PQ_LIB})
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE
                           _DBG_ LOGS_OUTPUT_TYPE=2 ERRORS_OUTPUT_TYPE=4)

# Output options
set(Configuration D)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX _V${CMAKE_PROJECT_VERSION}_${Configuration}.exe)
