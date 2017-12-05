################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################


list(APPEND FOUNDATION_HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfchronostream.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfchronostream.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdate.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdate.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdateclocks.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdatecomponent.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdatecomponent.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdateformatter.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdateformatter.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdatesymbol.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdatesymbol.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/date/tfdatetypes.hpp
        )

list(APPEND FOUNDATION_SOURCE_FILES
        src/date/tfchronostream.cxx
        src/date/tfchronostream.hxx
        src/date/tfdate.cxx
        src/date/tfdate.hxx
        src/date/tfdateclocks.hpp
        src/date/tfdatecomponent.cxx
        src/date/tfdatecomponent.hxx
        src/date/tfdateformatter.cxx
        src/date/tfdateformatter.hxx
        src/date/tfdatesymbol.cxx
        src/date/tfdatesymbol.hxx
        src/date/tfdatetypes.cpp
        src/date/tfdatetypes.hpp
        )


set_source_files_properties(
        "src/date/tfchronostream.cxx"
        "src/date/tfchronostream.hxx"
        "src/date/tfdate.cxx"
        "src/date/tfdate.hxx"
        "src/date/tfdatecomponent.cxx"
        "src/date/tfdatecomponent.hxx"
        "src/date/tfdateformatter.cxx"
        "src/date/tfdateformatter.hxx"
        "src/date/tfdatesymbol.cxx"
        "src/date/tfdatesymbol.hxx"
        PROPERTIES
        HEADER_FILE_ONLY ON
)