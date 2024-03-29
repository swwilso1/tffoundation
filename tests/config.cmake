################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include_directories(
	${TEST_INCLUDE_DIRECTORIES}
)

link_directories(
	${TEST_LIBRARY_DIRECTORIES}
)

include(tests/cmake/config.cmake)
include(tests/alarm/config.cmake)
include(tests/allocator/config.cmake)
include(tests/array/config.cmake)
include(tests/base64/config.cmake)
include(tests/commandline/config.cmake)
include(tests/copy/config.cmake)
include(tests/data/config.cmake)
include(tests/date/config.cmake)
include(tests/defer/config.cmake)
include(tests/dll/config.cmake)
include(tests/encoding/config.cmake)
include(tests/environment/config.cmake)
include(tests/files/config.cmake)
include(tests/format/config.cmake)
include(tests/log/config.cmake)
include(tests/multivalue/config.cmake)
include(tests/network/config.cmake)
include(tests/notification/config.cmake)
include(tests/numerics/config.cmake)
include(tests/queue/config.cmake)
include(tests/random/config.cmake)
include(tests/pipe/config.cmake)
include(tests/platform/config.cmake)
include(tests/poll/config.cmake)
include(tests/process/config.cmake)
include(tests/progress/config.cmake)
if (UNIX)
    include(tests/signal/config.cmake)
endif()
include(tests/sleep/config.cmake)
include(tests/string/config.cmake)
include(tests/symbol/config.cmake)
include(tests/template/config.cmake)
include(tests/thread/config.cmake)
include(tests/uuid/config.cmake)
include(tests/version/config.cmake)
