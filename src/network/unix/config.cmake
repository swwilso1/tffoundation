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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixnetworkutilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixnetworkutilities.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixsocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixsocket.hpp
    )

if (NOT APPLE)
    list(APPEND FOUNDATION_HEADER_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfnetlink.hpp
            )

    list(APPEND FOUNDATION_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfnetlink.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfnetlink.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tflinuxnetworkinterface.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tflinuxnetworkinterfaces.cpp
            )
else()
    list(APPEND FOUNDATION_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfapplenetworkinterface.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfapplenetworkinterfaces.cpp
            )
endif()
