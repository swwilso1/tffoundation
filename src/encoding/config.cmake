################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

list(APPEND FOUNDATION_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/encoding/tfencoding.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/encoding/tfencodingrecognizer.hpp"
    )

list(APPEND FOUNDATION_SOURCE_FILES
    src/encoding/tfasciiencodingteststrategy.cpp
    src/encoding/tfasciiencodingteststrategy.hpp
    src/encoding/tfencoding.cpp
    src/encoding/tfencoding.hpp
    src/encoding/tfencodingrecognizer.cpp
    src/encoding/tfencodingrecognizer.hpp
    src/encoding/tfencodingteststrategy.hpp
    src/encoding/tfutf16encodingteststrategy.cpp
    src/encoding/tfutf16encodingteststrategy.hpp
    src/encoding/tfutf32encodingteststrategy.cpp
    src/encoding/tfutf32encodingteststrategy.hpp
    src/encoding/tfutf8encodingteststrategy.cpp
    src/encoding/tfutf8encodingteststrategy.hpp
    src/encoding/tfwindows1252encodingteststrategy.cpp
    src/encoding/tfwindows1252encodingteststrategy.hpp
    )
