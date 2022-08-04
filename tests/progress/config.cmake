################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

build_and_run_test(
        basic_progress_notifier_test
        BasicProgressNotifierTest
        tests/progress/basic_progress_notifier_test.cpp
)

build_and_run_test(
        progress_test
        ProgressTest
        tests/progress/progress_bar_test.cpp
)

build_and_run_test(
        progress_meter
        ProgressMeter
        tests/progress/progress_meter_test.cpp
)
