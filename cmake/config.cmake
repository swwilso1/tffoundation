################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(cmake/headers.cmake)

set(GENERATED_SOURCES_DIR "generates_sources" CACHE STRING "Name of generated source directory")

set(FOUNDATION_LIBRARY_NAME "TFFoundation" CACHE STRING "Library name")

set(FOUNDATION_RUNTIME_INSTALL_DIR "bin" CACHE STRING "Executable object install directory.")

set(FOUNDATION_LIBRARY_INSTALL_DIR "lib" CACHE STRING "Library object install directory.")

set(FOUNDATION_ARCHIVE_INSTALL_DIR "${FOUNDATION_LIBRARY_INSTALL_DIR}" CACHE STRING "Archive library instal directory")

set(FOUNDATION_BUILD_DIR build CACHE STRING "Name of directory that contains exported .cmake files")


