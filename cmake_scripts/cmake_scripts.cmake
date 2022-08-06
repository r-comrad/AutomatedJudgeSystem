#--------------------------------------------------------------------------------

set(SCRIPT_NAMES add_sources prepare_file download_archive)

foreach(SCRIPT IN LISTS SCRIPT_NAMES)
    include(${CMAKE_CURRENT_LIST_DIR}/${SCRIPT}.cmake)
endforeach()

#--------------------------------------------------------------------------------
