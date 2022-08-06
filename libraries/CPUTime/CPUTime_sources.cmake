#--------------------------------------------------------------------------------

set (FILE_NAME ${CMAKE_CURRENT_LIST_DIR}/getCPUTime.cpp)
set (URL https://gist.githubusercontent.com/Randl/45bcca59720f661fa033a67d5f44bff0/raw/ba8764bdc4725f5c411181e65df7330d3abed9af/getCPUTime.c)
set (HASH 4cb9a79c06a627126d687d637a217e23c64ae163f7cda080edd35b27be1b0730)

prepare_file(${FILE_NAME} ${URL} ${HASH})

set(
    CPUTIME_SOURCES

    getCPUTime.cpp
    getCPUTime.hpp
)

#--------------------------------------------------------------------------------
