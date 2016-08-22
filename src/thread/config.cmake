################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/thread/tfthreadcontroller.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/thread/tfthreadsafequeue.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/thread/tfthreadsafequeue.hxx"
)


set_source_files_properties(
	"src/thread/tfthreadsafequeue.cxx"
	PROPERTIES
	HEADER_FILE_ONLY ON
)


list(APPEND FOUNDATION_SOURCE_FILES
	"src/thread/tfthreadcontroller.cpp"
	"src/thread/tfthreadcontroller.hpp"
	"src/thread/tfthreadsafequeue.cxx"
	"src/thread/tfthreadsafequeue.hxx"
)

