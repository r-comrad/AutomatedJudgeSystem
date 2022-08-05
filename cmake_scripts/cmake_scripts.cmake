#--------------------------------------------------------------------------------

set(SCRIPT_NAMES add_sources download_file)

foreach(SCRIPT IN LISTS SCRIPT_NAMES)
    message(${CMAKE_CURRENT_LIST_DIR}/${SCRIPT}.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/${SCRIPT}.cmake)
endforeach()

#--------------------------------------------------------------------------------
