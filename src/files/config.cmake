################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffilemanager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffilepermissions.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/files/tffileproperties.hpp
        )


list(APPEND FOUNDATION_SOURCE_FILES
        src/files/tffilemanager.cpp
        src/files/tffilemanager.hpp
        src/files/tffilepermissions.cpp
        src/files/tffilepermissions.hpp
        src/files/tffileproperties.cpp
        src/files/tffileproperties.hpp
        )