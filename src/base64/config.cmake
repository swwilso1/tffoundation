################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/base64/tfbase64.hpp"
        )

list(APPEND FOUNDATION_SOURCE_FILES
        src/base64/tfbase64.cpp
        src/base64/tfbase64.hpp
        )

