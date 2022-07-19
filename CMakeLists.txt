cmake_minimum_required(VERSION 3.21.3)
project(ChineseTeste VERSION 2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("/std:c++latest")
# if (MSVC_VERSION GREATER_EQUAL "1900")
#     include(CheckCXXCompilerFlag)
#     CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_latest_flag_supported)
#     if (_cpp_latest_flag_supported)
#         add_compile_options("/std:c++20")
#     endif()
# endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")
#find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

# include_directories(ChineseTesters libraries)

include(${CMAKE_CURRENT_SOURCE_DIR}/sources/sources.cmake)

get_sources(${CMAKE_CURRENT_SOURCE_DIR}/sources SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(libraries)

#target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sources)

target_compile_definitions(${PROJECT_NAME} PRIVATE
                           BILL_WINDOWS LOGS_OUTPUT_TYPE=1 ERRORS_OUTPUT_TYPE=4)
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BILL_WINDOWS)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE LINUS_LINUX)
endif()

set(Configuration D)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX _V${CMAKE_PROJECT_VERSION}_${Configuration}.exe)
