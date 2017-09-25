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
        StringInitFormatTest
        tests/string/init-format-test.cpp
)


build_and_run_test(
        iterator-test
        StringIteratorTest
        tests/string/iterator-test.cpp
)


build_and_run_test(
        equality-test
        StringEqualityTest
        tests/string/equality-tests.cpp
)


build_and_run_test(
        assignment-test
        StringAssignmentTest
        tests/string/assignment-test.cpp
)


build_and_run_test(
        length-test
        StringLengthTest
        tests/string/length-tests.cpp
)


build_and_run_test(
        get-character-test
        StringGetCharacterTest
        tests/string/get-character-tests.cpp
)


build_and_run_test(
        get-c-string-test
        StringGetCStringTest
        tests/string/get-c-string-tests.cpp
)


build_and_run_test(
        combining-strings-test
        StringCombiningStringsTest
        tests/string/combining-strings-tests.cpp
)


build_and_run_test(
        dividing-strings-test
        StringDividingStringsTest
        tests/string/dividing-strings-tests.cpp
)


build_and_run_test(
        finding-substrings-tests
        StringFindingSubstringsTest
        tests/string/finding-substrings-tests.cpp
)


build_and_run_test(
        replacing-substrings-tests
        StringReplaceSubstringsTest
        tests/string/replacing-substrings-tests.cpp
)


build_and_run_test(
        comparison-tests
        StringComparisonTests
        tests/string/comparison-tests.cpp
)


build_and_run_test(
        character-case-tests
        StringCharacterCaseTests
        tests/string/character-case-tests.cpp
)





