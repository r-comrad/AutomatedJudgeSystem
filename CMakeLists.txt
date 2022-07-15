cmake_minimum_required(VERSION 3.21.3)
project(ChineseTeste VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--no-as-needed -ldl")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(PQXX /usr/include/pqxx)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

# include_directories(ChineseTesters libraries)

include(${CMAKE_CURRENT_SOURCE_DIR}/sources/sources.cmake)

get_sources(${CMAKE_CURRENT_SOURCE_DIR}/sources SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PQXX_LIB} ${PQ_LIB})

add_subdirectory(libraries)

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sources)

target_compile_definitions(${PROJECT_NAME} PRIVATE
                            LINUS_LINUX LOGS_OUTPUT_TYPE=2 ERRORS_OUTPUT_TYPE=4)
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BILL_WINDOWS)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE LINUS_LINUX)
endif()

set(Configuration D)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX _V${CMAKE_PROJECT_VERSION}_${Configuration}.exe)
