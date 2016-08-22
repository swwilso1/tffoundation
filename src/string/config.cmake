################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################


list(APPEND FOUNDATION_HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/string/tfasciistringencoder.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/string/tfstringcase.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/string/tfstringencoder.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/string/tfstringtypes.hpp"
)


list(APPEND FOUNDATION_SOURCE_FILES
	"src/string/tfasciistringencoder.cpp"
	"src/string/tfasciistringencoder.hpp"
	"src/string/tfstringcase.hpp"
	"src/string/tfstringencoder.cpp"
	"src/string/tfstringencoder.hpp"
	"src/string/tfstringtypes.hpp"
)

