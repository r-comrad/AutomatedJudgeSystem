#--------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21.3)

set (CASH_TYPE SHA256)

#--------------------------------------------------------------------------------

function(download_file PATH_TO_FILE URL FILE_HASH)
    message("Downloading file ${PATH_TO_FILE}")
    file(DOWNLOAD ${URL} ${PATH_TO_FILE} EXPECTED_HASH 
        ${CASH_TYPE}=${FILE_HASH})
endfunction()

macro(check_file RESULT PATH_TO_FILE FILE_HASH)
    if (NOT EXISTS ${PATH_TO_FILE})
        message("File ${PATH_TO_FILE} doesn't exist")
        set(${RESULT} 0)
    elseif()
        file(${CASH_TYPE} ${PATH_TO_FILE} CURRENT_HASH) 
        if(NOT ${CURRENT_HASH} EQUAL ${FILE_HASH})
        message("File ${PATH_TO_FILE} corrupted")
            set(${RESULT} 0)
        else()
            set(${RESULT} 1)
        endif()
    endif()
endmacro()

function(prepare_file PATH_TO_FILE URL FILE_HASH)
    check_file(RESULT ${FILE_NAME} ${HASH})
    if(NOT ${RESULT})
        download_file(${FILE_NAME} ${URL} ${HASH})
    endif()
endfunction()

#--------------------------------------------------------------------------------
