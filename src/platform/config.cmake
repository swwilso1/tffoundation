################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/tfcompilerid.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/tfplatformid.hpp"
    )

list(APPEND FOUNDATION_SOURCE_FILES
    src/platform/tfcompilerid.cpp
    src/platform/tfcompilerid.hpp
    src/platform/tfplatformid.cpp
    src/platform/tfplatformid.hpp)

if (UNIX)
    include(src/platform/unix/config.cmake)
endif()

if (WIN32)
    include(src/platform/win32/config.cmake)
endif()
