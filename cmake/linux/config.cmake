################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG(-std=c++14 HAS_STDCXX14_FLAG)

if(HAS_STDCXX14_FLAG)
    list(APPEND LINUX_COMPILE_FLAGS -std=c++14)
else()
	CHECK_CXX_COMPILER_FLAG(-std=c++11 HAS_STDCXX11_FLAG)
	if(HAS_STDCXX11_FLAG)
		list(APPEND LINUX_COMPILE_FLAGS -std=c++11)
	endif()
endif()

set(FOUNDATION_SHARED_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-shared CACHE STRING
    "Name of shared library target")

set(FOUNDATION_STATIC_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-static CACHE STRING
    "Name of static library target")

mark_as_advanced(
    FOUNDATION_SHARED_LIBARY_NAME
    FOUNDATION_STATIC_LIBRARY_NAME
)

set(FOUNDATION_SHARED_LIBRARY_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})
set(FOUNDATION_STATIC_LIBRARY_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})
set(HEADER_TEST_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS} CACHE STRING "Flags for testing header files")
set(TEST_PLATFORM_COMPILE_FLAGS ${LINUX_COMPILE_FLAGS})

set(TEST_PLATFORM_LIBRARIES gtest gtest_main pthread rt)
