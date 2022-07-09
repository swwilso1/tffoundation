################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

build_and_run_test(
    signalset_tests
    SignalSetTests
    tests/signal/signalset_tests.cpp
)

build_and_run_test(
    signalhandler_tests
    SignalHandlerTests
    tests/signal/signalhandler_tests.cpp
)

build_and_run_test(
    signalmanager_tests
    SignalManagerTests
    tests/signal/signalmanager_tests.cpp
)
