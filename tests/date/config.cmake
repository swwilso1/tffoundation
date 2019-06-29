################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

build_and_run_test(
        date_test
        DateTest
        tests/date/date_test.cpp
)


build_and_run_test(
        date_components_test
        DateComponents
        tests/date/date_components_test.cpp
)


build_and_run_test(
        date_formatter_test
        DateFormatter
        tests/date/date_formatter_test.cpp
)

