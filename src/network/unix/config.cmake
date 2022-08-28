################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

#list(APPEND FOUNDATION_HEADER_FILES
#    )

list(APPEND FOUNDATION_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfnetworkinitialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixnetworkerror.cpp
    )
