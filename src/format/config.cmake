################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/format/tfclassformatter.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/format/tfclassformatter.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/format/tfclassmember.hxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/format/tfformatter.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/format/tfformatterfactory.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/format/tflogxmlclassformatter.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/format/tftab.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/format/tfxmlclassformatter.hpp"
)

set_source_files_properties(src/format/tfclassformatter.cxx PROPERTIES
	HEADER_FILE_ONLY ON
)

list(APPEND FOUNDATION_SOURCE_FILES
	src/format/tfclassformatter.cpp
	src/format/tfclassformatter.cxx
	src/format/tfclassformatter.hpp
	src/format/tfclassmember.hxx
	src/format/tfclassmember.cpp
	src/format/tfformatterfactory.cpp
	src/format/tfformatterfactory.hpp
	src/format/tflogxmlclassformatter.cpp
	src/format/tflogxmlclassformatter.hpp
	src/format/tftab.hpp
	src/format/tftab.cpp
	src/format/tfxmlclassformatter.cpp
	src/format/tfxmlclassformatter.hpp
)

