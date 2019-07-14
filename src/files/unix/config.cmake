################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

if(APPLE)
    list(APPEND FOUNDATION_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/files/unix/tfapplefilemanager.cpp
            )
else()
    list(APPEND FOUNDATION_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/files/unix/tflinuxfilemanager.cpp
            )
endif()


list(APPEND FOUNDATION_HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/unix/tfunixfilehandle.hpp
        )

list(APPEND FOUNDATION_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/unix/tfunixfilehandle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/unix/tfunixfilehandle.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/unix/tfunixfilemanager.cpp
        )

