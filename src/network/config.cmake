################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfipaddress.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfnetworkaddress.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfnetworkerror.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfnetworkinitialization.hpp
    )

list(APPEND FOUNDATION_SOURCE_FILES
    src/network/tfipaddress.cpp
    src/network/tfipaddress.hpp
    src/network/tfnetworkaddress.cpp
    src/network/tfnetworkerror.cpp
    src/network/tfnetworkerror.hpp
    src/network/tfnetworkinitialization.hpp
    )

if (UNIX)
    include(src/network/unix/config.cmake)
endif()

if (WIN32)
    include(src/network/win32/config.cmake)
endif()
