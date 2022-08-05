#--------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21.3)

#--------------------------------------------------------------------------------

function(download_file PATH_TO_FILE URL FILE_HASH)
    file(DOWNLOAD ${URL} ${PATH_TO_FILE} EXPECTED_HASH SHA256=${FILE_HASH})
endfunction()

function(check_existence PATH_TO_FILE URL FILE_HASH)
    if (NOT EXISTS ${PATH_TO_FILE})
        message(Dowloanding " " ${PATH_TO_FILE})   
        download_file(${PATH_TO_FILE} ${URL} ${FILE_HASH})
        return()
    endif()

    file(SHA256 ${PATH_TO_FILE} CURRENT_HASH) 
    if(NOT ${CURRENT_HASH} EQUAL ${FILE_HASH})
        message(Restoring " " ${PATH_TO_FILE})   
        download_file(${PATH_TO_FILE} ${URL} ${FILE_HASH})
    endif()
endfunction()

#--------------------------------------------------------------------------------
