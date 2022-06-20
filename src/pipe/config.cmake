################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pipe/tfpipe.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pipe/tfpipebase.hxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pipe/tfunixpipe.hpp"
        )

list(APPEND FOUNDATION_SOURCE_FILES
        src/pipe/tfpipe.hpp
        src/pipe/tfpipebase.hxx
        src/pipe/tfunixpipe.cpp
        src/pipe/tfunixpipe.hpp
        )
