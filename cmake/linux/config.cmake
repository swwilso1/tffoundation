################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

check_c_compiler_flag("-Wunknown-pragmas" HAVE_GCC_WARNING_UNKNOWN_PRAGMAS)

set(FOUNDATION_SHARED_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-shared CACHE STRING
    "Name of shared library target")

set(FOUNDATION_STATIC_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-static CACHE STRING
    "Name of static library target")

mark_as_advanced(
    FOUNDATION_SHARED_LIBARY_NAME
    FOUNDATION_STATIC_LIBRARY_NAME
)

list(APPEND LINUX_COMPILE_FLAGS -Wall -Wno-unknown-pragmas -Wextra -Wconversion -Wsign-conversion)
#list(APPEND LINUX_COMPILE_FLAGS -Werror -pedantic-errors)

if (BUILD_PROFILE)
    list(APPEND LINUX_COMPILE_FLAGS -pg)
endif()

if (BUILD_SANITIZER)
    list(APPEND LINUX_COMPILE_FLAGS -fsanitize=address -fno-omit-frame-pointer)
endif()

set(LINUX_LIBRARIES pthread rt uuid)
set(FOUNDATION_SHARED_LIBRARY_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})
set(FOUNDATION_SHARED_LIBRARY_PLATFORM_LIBRARIES ${LINUX_LIBRARY})
set(FOUNDATION_STATIC_LIBRARY_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})
set(FOUNDATION_STATIC_LIBRARY_PLATFORM_LIBRARIES ${LINUX_LIBRARIES})
set(HEADER_TEST_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS} CACHE STRING "Flags for testing header files")
set(TEST_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})

set(TEST_PLATFORM_LIBRARIES pthread rt uuid)

