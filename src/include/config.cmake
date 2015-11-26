################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include/tfassert.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include/tfheaders.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/include/tftypes.hpp"
    "${GENERATED_SOURCES_DIR}/tfplatformheaders.h"
)


list(APPEND FOUNDATION_SOURCE_FILES
    "src/include/tfassert.hpp"
    "src/include/tfheaders.hpp"
	"src/include/tftypes.hpp"
    "${GENERATED_SOURCES_DIR}/tfplatformheaders.h"
)


file(MAKE_DIRECTORY "${GENERATED_SOURCES_DIR}")

configure_file("src/include/tfplatformheaders.h.in"
        "${GENERATED_SOURCES_DIR}/tfplatformheaders.h"
)

