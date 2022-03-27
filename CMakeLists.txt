cmake_minimum_required(VERSION 3.21)
project(SubmissionTester VERSION 0.95)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_compile_definitions(WIN_BY_ME)

INCLUDE_DIRECTORIES("libraries")

add_executable(${PROJECT_NAME} "sources/main/main.cpp")

find_package(PostgreSQL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PostgreSQL_LIBRARIES})

add_subdirectory("libraries")
add_subdirectory("sources")

set(Configuration "D")

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_NewVegasEdition")
set_target_properties(${PROJECT_NAME}  PROPERTIES SUFFIX "_V${CMAKE_PROJECT_VERSION}_${Configuration}.exe")