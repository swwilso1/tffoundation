################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

build_and_run_test(
        string-construction-test
        StringConstructionTest
        tests/string/string-construction-test.cpp
)


build_and_run_test(
        init-format-test
        InitFormatTest
        tests/string/init-format-test.cpp
)


build_and_run_test(
        iterator-test
        IteratorTest
        tests/string/iterator-test.cpp
)


build_and_run_test(
        equality-test
        EqualityTest
        tests/string/equality-tests.cpp
)