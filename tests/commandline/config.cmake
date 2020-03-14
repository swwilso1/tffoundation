################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

build_and_run_test(
    argument_test
    ArgumentTest
    tests/commandline/argument_test.cpp
)

build_and_run_test(
    argument_parser_test
    ArgumentParserTest
    tests/commandline/argument_parser_test.cpp
)

