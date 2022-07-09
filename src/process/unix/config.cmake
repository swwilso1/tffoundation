################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/process/unix/tfprocess.hpp
    )

list(APPEND FOUNDATION_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/process/unix/tfprocess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/process/unix/tfprocess.hpp
    )
