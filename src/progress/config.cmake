################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/progress/tfprogressbar.hpp"
        )

list(APPEND FOUNDATION_SOURCE_FILES
        "src/progress/tfprogressbar.cpp"
        "src/progress/tfprogressbar.hpp"
        )
