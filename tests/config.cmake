################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include_directories(
	${GTEST_DIR}/include
	${TEST_INCLUDE_DIRECTORIES}
)

link_directories(
	${GTEST_DIR}/lib
	${TEST_LIBRARY_DIRECTORIES}
)

include(tests/cmake/config.cmake)
include(tests/allocator/config.cmake)
include(tests/array/config.cmake)
include(tests/data/config.cmake)
include(tests/format/config.cmake)
include(tests/notification/config.cmake)


