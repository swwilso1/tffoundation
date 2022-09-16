################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################



#list(APPEND FOUNDATION_HEADER_FILES
#    ${CMAKE_CURRENT_SOURCE_DIR}/src/network/unix/tfposixsocket.hpp
#    )

list(APPEND FOUNDATION_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/unix/tfposixplatformutilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/unix/tfposixplatformutilities.hpp
    )

if (APPLE)
    list(APPEND FOUNDATION_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/unix/tfappleplatformid.cpp)
else()
    list(APPEND FOUNDATION_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/unix/tflinuxplatformid.cpp)
endif()
