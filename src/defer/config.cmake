################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/defer/tfdefer.hpp"
        )


list(APPEND FOUNDATION_SOURCE_FILES
        src/defer/tfdefer.cpp
        src/defer/tfdefer.hpp
        )
