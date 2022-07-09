################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

build_and_run_test(
    command_line_tests
    CommandLineTests
    tests/process/command_line_tests.cpp
)

build_and_run_test(
    process_tests
    ProcessTests
    tests/process/process_tests.cpp
)