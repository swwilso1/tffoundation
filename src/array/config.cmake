################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/array/tfarray.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/array/tfarray.hxx"
        )


list(APPEND FOUNDATION_SOURCE_FILES
        src/array/tfarray.cxx
        src/array/tfarray.hxx
        )

set_source_files_properties(
        "src/array/tfarray.cxx"
        "src/array/tfarrah.hxx"
        PROPERTIES
        HEADER_FILE_ONLY ON
)

