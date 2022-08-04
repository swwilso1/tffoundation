################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/progress/tfbasicprogressnotifier.hxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/progress/tfprogressbar.hxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/progress/tfprogressmeter.hxx"
        )

list(APPEND FOUNDATION_SOURCE_FILES
        "src/progress/tfbasicprogressnotifier.hxx"
        "src/progress/tfprogressbar.hxx"
        "src/progress/tfprogressmeter.hxx"
        )
