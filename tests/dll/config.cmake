################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

add_library(dlltest MODULE tests/dll/dll_test_library.cpp)

build_and_run_test(
        dll_test
        DLLTest
        tests/dll/dll_test.cpp
)
