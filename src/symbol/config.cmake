################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/symbol/tfsymboltable.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/symbol/tfsymboltable.hxx"
)

set_source_files_properties(
    src/symbol/tfsymboltable.cxx
    src/symbol/tfsymboltable.hxx
    PROPERTIES
    HEADER_FILE_ONLY ON
)

list(APPEND FOUNDATION_SOURCE_FILES
    src/symbol/tfsymboltable.cxx
    src/symbol/tfsymboltable.hxx
)
