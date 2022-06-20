################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffilehandle.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffilehandlebase.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffilehandlebase.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffilemanager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffilepermissions.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffileproperties.hpp
        )

set_source_files_properties(
        "src/files/tffilehandlebase.cxx"
        "src/files/tffilehandlebase.hxx"
        PROPERTIES
        HEADER_FILE_ONLY ON
)


list(APPEND FOUNDATION_SOURCE_FILES
        src/files/tffilehandle.hpp
        src/files/tffilehandlebase.cxx
        src/files/tffilehandlebase.hxx
        src/files/tffilemanager.cpp
        src/files/tffilemanager.hpp
        src/files/tffilepermissions.cpp
        src/files/tffilepermissions.hpp
        src/files/tffileproperties.cpp
        src/files/tffileproperties.hpp
        )

if(UNIX)
    include(src/files/unix/config.cmake)
endif()

if(WIN32)
    include(src/files/win32/config.cmake)
endif()

