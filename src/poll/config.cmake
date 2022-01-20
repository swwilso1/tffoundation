################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/poll/tfpollevent.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/poll/tfpollworker.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/poll/tfunixpollworker.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/poll/tfunixselectworker.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/poll/tfpollerbase.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/poll/tfpoll.hpp"
    )

list(APPEND FOUNDATION_SOURCE_FILES
    )
