################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(CheckIncludeFile)
include(CheckIncludeFileCXX)

# Standard C header files

CHECK_INCLUDE_FILE(assert.h HAS_ASSERT_H)
CHECK_INCLUDE_FILE(limits.h HAS_LIMITS_H)
CHECK_INCLUDE_FILE(stdlib.h HAS_STDLIB_H)

# Standard C++ header files

CHECK_INCLUDE_FILE_CXX(cstddef HAS_CSTDDEF)
CHECK_INCLUDE_FILE_CXX(list HAS_LIST)
CHECK_INCLUDE_FILE_CXX(map HAS_MAP)
CHECK_INCLUDE_FILE_CXX(mutex HAS_MUTEX)
CHECK_INCLUDE_FILE_CXX(new HAS_NEW)
CHECK_INCLUDE_FILE_CXX(ostream HAS_OSTREAM)
CHECK_INCLUDE_FILE_CXX(sstream HAS_SSTREAM)
CHECK_INCLUDE_FILE_CXX(string HAS_STRING)
CHECK_INCLUDE_FILE_CXX(typeinfo HAS_TYPEINFO)
CHECK_INCLUDE_FILE_CXX(utility HAS_UTILITY)
CHECK_INCLUDE_FILE_CXX(vector HAS_VECTOR)

