################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/allocator/tfallocatorinterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/allocator/tfcxxallocator.hpp"
    )


list(APPEND FOUNDATION_SOURCE_FILES
    "src/allocator/tfallocatorinterface.cpp"
    "src/allocator/tfallocatorinterface.hpp"
    "src/allocator/tfcxxallocator.cpp"
    "src/allocator/tfcxxallocator.hpp"
    )

