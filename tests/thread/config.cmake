################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

build_and_run_test(
        threadsafeunorderedmap_test
        ThreadSafeUnorderedMapTest
        tests/thread/threadsafeunorderedmap_test.cpp
)

build_and_run_test(
    threadpool_test
    ThreadPoolTest
    tests/thread/threadpool_test.cpp
)
