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
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfmap.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfmap.hxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfmutex.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfpair.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfpair.hxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfqueue.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfqueue.hxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tftypetraits.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfunordered_map.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfunordered_map.hxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfvector.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tfvector.hxx"
)


set_source_files_properties(
	src/stl/tflist.cxx
	src/stl/tfmap.cxx
	src/stl/tfpair.cxx
	src/stl/tfqueue.cxx
	src/stl/tfunordered_map.cxx
	src/stl/tfvector.cxx
	PROPERTIES
	HEADER_FILE_ONLY ON
)


list(APPEND FOUNDATION_SOURCE_FILES
	src/stl/tfconditionvariable.cpp
	src/stl/tfconditionvariable.hpp
	src/stl/tflist.cxx
	src/stl/tflist.hxx
	src/stl/tfmap.cxx
	src/stl/tfmap.hxx
	src/stl/tfmutex.cpp
	src/stl/tfmutex.hpp
	src/stl/tfpair.cxx
	src/stl/tfpair.hxx
	src/stl/tfqueue.cxx
	src/stl/tfqueue.hxx
	src/stl/tftypetraits.hpp
	src/stl/tfunordered_map.cxx
	src/stl/tfunordered_map.hxx
	src/stl/tfvector.cxx
	src/stl/tfvector.hxx
)

