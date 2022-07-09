################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/process/tfcommandline.hpp"
    )

list(APPEND FOUNDATION_SOURCE_FILES
    "src/process/tfcommandline.cpp"
    "src/process/tfcommandline.hpp"
    )

if(UNIX)
    include(src/process/unix/config.cmake)
endif()

if(WIN32)
    include(src/process/win32/config.cmake)
endif()
