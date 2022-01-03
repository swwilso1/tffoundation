################################################################################
#####
##### Tectiform TXFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

cmake_minimum_required(VERSION 3.21)

project(TFFoundation)

set(TFFoundation_VERSION 1.0.0)

cmake_policy(SET CMP0075 OLD)

include(cmake/config.cmake)
include(src/config.cmake)

option(BUILD_TESTS "create library test programs" OFF)

if(BUILD_TESTS)
	if(NOT GTEST_DIR)
		message(FATAL_ERROR "Tests require GTest headers and libraries")
	endif()
	enable_testing()
	include(tests/config.cmake)
endif()

include(CPack)

