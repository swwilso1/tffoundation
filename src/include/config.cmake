################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include/tfassert.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include/tfcomparison.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include/tfheaders.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include/tfinternalexceptions.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/include/tftypes.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/include/TFFoundation.hpp"
    "${GENERATED_SOURCES_DIR}/tfplatformheaders.h"
)


list(APPEND FOUNDATION_SOURCE_FILES
    "src/include/tfassert.hpp"
    "src/include/tfcomparison.hpp"
    "src/include/tfheaders.hpp"
    "src/include/tfinternalexceptions.hpp"
    "src/include/tftypes.cpp"
	"src/include/tftypes.hpp"
	"src/include/TFFoundation.hpp"
    "${GENERATED_SOURCES_DIR}/tfplatformheaders.h"
)


file(MAKE_DIRECTORY "${GENERATED_SOURCES_DIR}")

configure_file("src/include/tfplatformheaders.h.in"
        "${GENERATED_SOURCES_DIR}/tfplatformheaders.h"
)

