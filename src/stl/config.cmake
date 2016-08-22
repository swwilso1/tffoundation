################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfconditionvariable.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tflist.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tflist.hxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfmutex.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfqueue.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfqueue.hxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfthread.hpp"
)


set_source_files_properties(
	src/stl/tflist.cxx
	src/stl/tfqueue.cxx
	PROPERTIES
	HEADER_FILE_ONLY ON
)


list(APPEND FOUNDATION_SOURCE_FILES
	src/stl/tfconditionvariable.cpp
	src/stl/tfconditionvariable.hpp
	src/stl/tflist.cxx
	src/stl/tflist.hxx
	src/stl/tfmutex.cpp
	src/stl/tfmutex.hpp
	src/stl/tfqueue.cxx
	src/stl/tfqueue.hxx
	src/stl/tfthread.cpp
	src/stl/tfthread.hpp
)

