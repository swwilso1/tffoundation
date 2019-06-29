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


list(APPEND FOUNDATION_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/unix/tfunixfilemanager.cpp
        )

