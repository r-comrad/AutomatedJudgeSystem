# cmake_minimum_required(VERSION 3.21)

# add_subdirectory(SQLite)
# target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
# target_link_libraries(${PROJECT_NAME} PUBLIC SQLite)

# add_subdirectory(CPUTime)
# target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
# target_link_libraries(${PROJECT_NAME} PUBLIC CPUTime)

cmake_minimum_required(VERSION 3.21)

set(DIRS SQLite CPUTime)
foreach(DIR IN LISTS DIRS)
    add_subdirectory(${DIR})
    target_include_directories(${DIR} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${DIR})
endforeach()