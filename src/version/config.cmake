################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version/tfversion.hpp"
    )

list(APPEND FOUNDATION_SOURCE_FILES
    src/version/tfversion.cpp
    src/version/tfversion.hpp)
