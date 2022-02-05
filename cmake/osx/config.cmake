################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

check_c_compiler_flag("-Wunknown-pragmas" HAVE_GCC_WARNING_UNKNOWN_PRAGMAS)

set(FOUNDATION_FRAMEWORK_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-framework CACHE STRING
    "Name of framework library target")

set(FOUNDATION_SHARED_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-shared CACHE STRING
    "Name of shared library target")

set(FOUNDATION_STATIC_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-static CACHE STRING
    "Name of static library target")

set(FOUNDATION_FRAMEWORK_INSTALL_DIR "${FOUNDATION_LIBRARY_INSTALL_DIR}" CACHE STRING
    "Framework object install directory.")

mark_as_advanced(
    FOUNDATION_FRAMEWORK_LIBRARY_NAME
    FOUNDATION_SHARED_LIBARY_NAME
    FOUNDATION_STATIC_LIBRARY_NAME
    FOUNDATION_FRAMEWORK_INSTALL_DIR
)

list(APPEND OSX_COMPILE_FLAGS -stdlib=libc++)
list(APPEND OSX_COMPILE_FLAGS -Werror -Wall -Wno-unknown-pragmas -Wextra -Wconversion -Wsign-conversion)
# list(APPEND OSX_COMPILE_FLAGS -Werror -pedantic-errors)

if (BUILD_PROFILE)
    list(APPEND OSX_COMPILE_FLAGS -pg)
endif()

if (BUILD_SANITIZER)
    list(APPEND OSX_COMPILE_FLAGS -fsanitize=address -fno-omit-frame-pointer)
endif()

# set(FOUNDATION_FRAMEWORK_LIBRARY_COMPILE_DEFINITIONS )
set(FOUNDATION_FRAMEWORK_LIBRARY_COMPILE_FLAGS ${OSX_COMPILE_FLAGS})
# set(FOUNDATION_FRAMEWORK_LIBRARY_LINK_FLAGS )
# set(FOUNDATION_FRAMEWORK_LIBRARY_LIBRARIES )

# set(FOUNDATION_SHARED_LIBRARY_PLATFORM_COMPILE_DEFINITIONS )
set(FOUNDATION_SHARED_LIBRARY_PLATFORM_COMPILE_FLAGS ${OSX_COMPILE_FLAGS})
# set(FOUNDATION_SHARED_LIBRARY_PLATFORM_LINK_FLAGS )
# set(FOUNDATION_SHARED_LIBRARY_PLATFORM_LIBRARIES )

# set(FOUNDATION_STATIC_LIBRARY_PLATFORM_COMPILE_DEFINITIONS )
set(FOUNDATION_STATIC_LIBRARY_PLATFORM_COMPILE_FLAGS ${OSX_COMPILE_FLAGS})
# set(FOUNDATION_STATIC_LIBRARY_PLATFORM_LINK_FLAGS )
# set(FOUNDATION_STATIC_LIBRARY_PLATFORM_LIBRARIES )


# set(TEST_PLATFORM_COMPILE_DEFINITIONS)
set(TEST_PLATFORM_COMPILE_FLAGS ${OSX_COMPILE_FLAGS})
# set(TEST_PLATFORM_LINK_FLAGS )
# set(TEST_PLATFORM_LIBRARIES )
