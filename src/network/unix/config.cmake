################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixsocket.hpp
    )

list(APPEND FOUNDATION_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfnetworkinitialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixnetworkerror.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixsocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixsocket.hpp
    )
