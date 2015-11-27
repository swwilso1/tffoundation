################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(cmake/headers.cmake)

set(GENERATED_SOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_sources" CACHE STRING "Name of generated source directory")

set(FOUNDATION_LIBRARY_NAME "TFFoundation" CACHE STRING "Library name")

set(FOUNDATION_RUNTIME_INSTALL_DIR "bin" CACHE STRING "Executable object install directory.")

set(FOUNDATION_LIBRARY_INSTALL_DIR "lib" CACHE STRING "Library object install directory.")

set(FOUNDATION_ARCHIVE_INSTALL_DIR "${FOUNDATION_LIBRARY_INSTALL_DIR}" CACHE STRING "Archive library instal directory")

set(FOUNDATION_BUILD_DIR build CACHE STRING "Name of directory that contains exported .cmake files")

mark_as_advanced(
    FOUNDATION_RUNTIME_INSTALL_DIR
    FOUNDATION_LIBRARY_INSTALL_DIR
    FOUNDATION_ARCHIVE_INSTALL-DIR
    FOUNDATION_BUILD_DIR
)

if(APPLE)
    include(cmake/osx/config.cmake)
else()
    include(cmake/linux/config.cmake)
endif()

set(FOUNDATION_SHARED_LIBRARY_COMPILE_DEFINITIONS ${FOUNDATION_SHARED_LIBRARY_PLATFORM_COMPILE_DEFINITIONS})
set(FOUNDATION_SHARED_LIBRARY_COMPILE_FLAGS ${FOUNDATION_SHARED_LIBRARY_PLATFORM_COMPILE_FLAGS})
set(FOUNDATION_SHARED_LIBRARY_LINK_FLAGS ${FOUNDATION_SHARED_LIBRARY_PLATFORM_LINK_FLAGS})
set(FOUNDATION_SHARED_LIBRARY_LIBRARIES ${FOUNDATION_SHARED_LIBRARY_PLATFORM_LIBRARIES})

set(FOUNDATION_STATIC_LIBRARY_COMPILE_DEFINITIONS ${FOUNDATION_STATIC_LIBRARY_PLATFORM_COMPILE_DEFINITIONS})
set(FOUNDATION_STATIC_LIBRARY_COMPILE_FLAGS ${FOUNDATION_STATIC_LIBRARY_PLATFORM_COMPILE_FLAGS})
set(FOUNDATION_STATIC_LIBRARY_LINK_FLAGS ${FOUNDATION_STATIC_LIBRARY_PLATFORM_LINK_FLAGS})
set(FOUNDATION_STATIC_LIBRARY_LIBRARIES ${FOUNDATION_STATIC_LIBRARY_PLATFORM_LIBRARIES})

set(TEST_COMPILE_DEFINITIONS ${TEST_PLATFORM_COMPILE_DEFINITIONS})
set(TEST_COMPILE_FLAGS ${TEST_PLATFORM_COMPILE_FLAGS})
set(TEST_INCLUDE_DIRECTORIES ${TEST_PLATFORM_INCLUDE_DIRECTORIES})
set(TEST_LIBRARY_DIRECTORIES ${TEST_PLATFORM_LIBRARY_DIRECTORIES})
set(TEST_LINK_FLAGS ${TEST_PLATFORM_LINK_FLAGS})
set(TEST_LIBRARIES ${TEST_PLATFORM_LIBRARIES})

list(APPEND TEST_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}/headers)
