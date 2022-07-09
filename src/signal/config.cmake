################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal/tfsignalhandler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal/tfsignalhandlerheaders.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal/tfsignalmanager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal/tfsignalset.hpp
    )

list(APPEND FOUNDATION_SOURCE_FILES
    src/signal/tfsignalhandler.cpp
    src/signal/tfsignalhandler.hpp
    src/signal/tfsignalhandlerheaders.hpp
    src/signal/tfsignalmanager.cpp
    src/signal/tfsignalmanager.hpp
    src/signal/tfsignalset.cpp
    src/signal/tfsignalset.hpp
    )
