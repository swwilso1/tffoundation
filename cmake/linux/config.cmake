################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

set(FOUNDATION_SHARED_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-shared CACHE STRING
    "Name of shared library target")

set(FOUNDATION_STATIC_LIBRARY_NAME ${FOUNDATION_LIBRARY_NAME}-static CACHE STRING
    "Name of static library target")

mark_as_advanced(
    FOUNDATION_SHARED_LIBARY_NAME
    FOUNDATION_STATIC_LIBRARY_NAME
)
