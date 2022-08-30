################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfinternetaddress.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfipaddress.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfnetworkaddress.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfnetworkerror.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfnetworkinitialization.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfnetworkinterface.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfsocket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/tfsocketbase.hxx
    )

set_source_files_properties(
    "src/network/tfsocketbase.hxx"
    PROPERTIES
    HEADER_FILE_ONLY ON
)

list(APPEND FOUNDATION_SOURCE_FILES
    src/network/tfinternetaddress.cpp
    src/network/tfinternetaddress.hpp
    src/network/tfipaddress.cpp
    src/network/tfipaddress.hpp
    src/network/tfnetworkaddress.cpp
    src/network/tfnetworkerror.cpp
    src/network/tfnetworkerror.hpp
    src/network/tfnetworkinitialization.hpp
    src/network/tfnetworkinterface.cpp
    src/network/tfnetworkinterface.hpp
    src/network/tfsocket.hpp
    src/network/tfsocketbase.hxx
    )

if (UNIX)
    include(src/network/unix/config.cmake)
endif()

if (WIN32)
    include(src/network/win32/config.cmake)
endif()
