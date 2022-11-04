################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/dll/tfdll.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/dll/tfdllbase.hxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/dll/tfdllloader.hpp"
        )


list(APPEND FOUNDATION_SOURCE_FILES
        src/dll/tfdll.hpp
        src/dll/tfdllbase.hxx
        src/dll/tfdllloader.hpp
        )

if (UNIX)
    include(src/dll/unix/config.cmake)
endif()

if (WIN32)
    include(src/dll/win32/config.cmake)
endif()
