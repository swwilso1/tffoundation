################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/commandline/tfargument.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/commandline/tfargumentaction.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/commandline/tfargumentparser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/commandline/tfargumenttype.hpp"
)

list(APPEND FOUNDATION_SOURCE_FILES
    src/commandline/tfargument.cpp
    src/commandline/tfargument.hpp
    src/commandline/tfargumentaction.cpp
    src/commandline/tfargumentaction.hpp
    src/commandline/tfargumentparser.cpp
    src/commandline/tfargumentparser.hpp
    src/commandline/tfargumenttype.cpp
    src/commandline/tfargumenttype.hpp
)

