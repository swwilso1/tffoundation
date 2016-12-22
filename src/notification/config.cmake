################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/notification/tfnotification.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/notification/tfnotificationcenter.hpp"
        )


list(APPEND FOUNDATION_SOURCE_FILES
        src/notification/tfnotification.cpp
        src/notification/tfnotification.hpp
        src/notification/tfnotificationcenter.cpp
        src/notification/tfnotificationcenter.hpp
 )
