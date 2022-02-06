################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

build_and_run_test(
        log_test
        LogTest
        tests/log/log_test.cpp
)

build_and_run_test(
    logrecord_test
    LogRecordTest
    tests/log/logrecord_test.cpp
)
