################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(CheckCXXCompilerFlag)

set(FOUNDATION_SHARED_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-shared CACHE STRING
    "Name of shared library target")

set(FOUNDATION_STATIC_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-static CACHE STRING
    "Name of static library target")

mark_as_advanced(
    FOUNDATION_SHARED_LIBARY_NAME
    FOUNDATION_STATIC_LIBRARY_NAME
)

set(LINUX_LIBRARIES pthread rt uuid)
set(FOUNDATION_SHARED_LIBRARY_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})
set(FOUNDATION_SHARED_LIBRARY_PLATFORM_LIBRARIES ${LINUX_LIBRARY})
set(FOUNDATION_STATIC_LIBRARY_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})
set(FOUNDATION_STATIC_LIBRARY_PLATFORM_LIBRARIES ${LINUX_LIBRARIES})
set(HEADER_TEST_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS} CACHE STRING "Flags for testing header files")
set(TEST_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})

set(TEST_PLATFORM_LIBRARIES pthread rt uuid)

