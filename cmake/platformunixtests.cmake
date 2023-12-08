################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(CheckIncludeFileCXX)
include(CheckTypeSize)
include(CheckSymbolExists)
include(CheckSourceCompiles)

check_include_file_cxx(uuid/uuid.h HAVE_UUID_H)
if(NOT HAVE_UUID_H)
  message(FATAL_ERROR "Can not find uuid/uuid.h, libuuid is necessary for the library")
endif()

check_type_size(uuid_string_t UUID_STRING_T)

list(APPEND CMAKE_REQUIRED_LIBRARIES uuid)
check_symbol_exists(uuid_unparse_upper uuid/uuid.h HAVE_UUID_UNPARSE_UPPER)

set(TIMEPOINT_SOURCE_CODE "#include <chrono>
#include <sstream>

int main()
{
std::stringstream s{};
auto right_now = std::chrono::system_clock::now();
s << right_now;
return 0;
}
")

check_source_compiles(CXX "${TIMEPOINT_SOURCE_CODE}" TIMEPOINT_HAS_OSTREAM_OUTPUT_OPERATOR)
