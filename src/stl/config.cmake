################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tflist.cxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/stl/tflist.hxx"
)


set_source_files_properties(
	src/stl/tflist.cxx
	PROPERTIES
	HEADER_FILE_ONLY ON
)


list(APPEND FOUNDATION_SOURCE_FILES
	src/stl/tflist.cxx
	src/stl/tflist.hxx
)

