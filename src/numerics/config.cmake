################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/numerics/tfconversion.hpp"
        )


list(APPEND FOUNDATION_SOURCE_FILES
        "src/numerics/tfconversion.cpp"
        "src/numerics/tfconversion.hpp"
        )
