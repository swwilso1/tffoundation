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


build_and_run_test(
        assignment-test
        AssignmentTest
        tests/string/assignment-test.cpp
)


build_and_run_test(
        length-test
        LengthTest
        tests/string/length-tests.cpp
)


build_and_run_test(
        get-character-test
        GetCharacterTest
        tests/string/get-character-tests.cpp
)


build_and_run_test(
        get-c-string-test
        GetCStringTest
        tests/string/get-c-string-tests.cpp
)


build_and_run_test(
        combining-strings-test
        CombiningStringsTest
        tests/string/combining-strings-tests.cpp
)

