################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/allocator/tfallocator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/allocator/tfallocatorinterface.hpp"
    "${GENERATED_SOURCES_DIR}/tfuseallocator.hpp"
    )

if(NOT SKIP_CXX_ALLOCATORS)
	list(APPEND FOUNDATION_HEADER_FILES
		"${CMAKE_CURRENT_SOURCE_DIR}/src/allocator/tfcxxallocator.hpp"
	)
endif()


list(APPEND FOUNDATION_SOURCE_FILES
    "src/allocator/tfallocator.hpp"
    "src/allocator/tfallocatorinterface.cpp"
    "src/allocator/tfallocatorinterface.hpp"
    "${GENERATED_SOURCES_DIR}/tfuseallocator.hpp"
    )

if(NOT SKIP_CXX_ALLOCATORS)
	list(APPEND FOUNDATION_SOURCE_FILES
		"src/allocator/tfcxxallocator.cpp"
		"src/allocator/tfcxxallocator.hpp"
	)
endif()

file(MAKE_DIRECTORY "${GENERATED_SOURCES_DIR}")

configure_file("src/allocator/tfuseallocator.hpp.in"
        "${GENERATED_SOURCES_DIR}/tfuseallocator.hpp"
)


