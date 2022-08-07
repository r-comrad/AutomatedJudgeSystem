#--------------------------------------------------------------------------------

set(
    SQLITE_SOURCES

    shell.c
    sqlite3.c

    sqlite3.h
    sqlite3ext.h
)

# foreach(FILE IN LISTS SQLITE_SOURCES)
#     list (APPEND FILE_NAMES ${CMAKE_CURRENT_LIST_DIR}/${FILE})
# endforeach()

set(
    FILE_HASHS

    2261db53df044cc42e62a2117d541536ca95838996084c970be4c8b83a301feb
    7cdf4718f314872b2b8cb6aeeaa60170a3bcf59f7e90ccbc41559718f8bf044b

    e43e15069a610488028afd095a1cb719c36fe0648a6a084e18910fae60cea4ea
    7f9ae3bab94bbc62d909fbb0149861b4a0832299f12d7ccbfecc7f28a8d27aa2
)

set (URL https://www.sqlite.org/2022/sqlite-amalgamation-3390200.zip)


list(GET  SQLITE_SOURCES 1 RESULT)

prepare_lib(${URL} ${CMAKE_CURRENT_LIST_DIR} "${SQLITE_SOURCES}" "${FILE_HASHS}" )


# get_corrupt_files(RESULT FILE_NAMES FILE_HASHS)

# list (LENGTH RESULT SIZE)
# if(${SIZE})
#     download_archive(${CMAKE_CURRENT_BINARY_DIR}/temp_lib ${URL})

#     foreach(NUM RANGE ${SIZE})
#     list(GET RESULT ${NUM} CORRUPT_FILE)

#     list(GET FILE_NAMES ${CORRUPT_FILE} FILE_PATH)
#     list(GET SQLITE_SOURCES ${CORRUPT_FILE} FILE)

#     message("Copy file ${FILE} to ${CMAKE_CURRENT_LIST_DIR}")
#     file(COPY_FILE ${CMAKE_CURRENT_BINARY_DIR}/temp_lib/${FILE}
#         ${FILE_PATH})

#     endforeach(NUM RANGE)

# endif()


#--------------------------------------------------------------------------------

# foreach(FILE IN LISTS FILE_NAMES)
#     file(${CASH_TYPE} ${FILE} CURRENT_HASH) 
#     message(--->${FILE})
#     message(--->${CURRENT_HASH})
# endforeach()
# [cmake] --->shell.c
# [cmake] --->2261db53df044cc42e62a2117d541536ca95838996084c970be4c8b83a301feb
# [cmake] --->sqlite3.c
# [cmake] --->7cdf4718f314872b2b8cb6aeeaa60170a3bcf59f7e90ccbc41559718f8bf044b
# [cmake] --->sqlite3.h
# [cmake] --->e43e15069a610488028afd095a1cb719c36fe0648a6a084e18910fae60cea4ea
# [cmake] --->sqlite3ext.h
# [cmake] --->7f9ae3bab94bbc62d909fbb0149861b4a0832299f12d7ccbfecc7f28a8d27aa2
