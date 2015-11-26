################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(src/base64/config.cmake)
include(src/data/config.cmake)
include(src/date/config.cmake)
include(src/include/config.cmake)
include(src/log/config.cmake)
include(src/string/config.cmake)
include(src/uuid/config.cmake)

include_directories(
    src/base64
    src/data
    src/date
    src/include
    src/log
    src/string
    src/uuid
    "${GENERATED_SOURCES_DIR}"
)


if(APPLE)
    add_library(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} SHARED ${FOUNDATION_SOURCE_FILES})
    set_target_properties(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} PROPERTIES
        FRAMEWORK ON
        OUTPUT_NAME ${FOUNDATION_LIBRARY_NAME}
        PUBLIC_HEADER "${FOUNDATION_HEADER_FILES}"
    )
    if(FOUNDATION_FRAMEWORK_LIBRARY_COMPILE_FLAGS)
        target_compile_options(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} PRIVATE
            ${FOUNDATION_FRAMEWORK_LIBRARY_COMPILE_FLAGS}
        )
    endif()

    install(TARGETS ${FOUNDATION_FRAMEWORK_LIBRARY_NAME}
        EXPORT ${FOUNDATION_LIBRARY_NAME}
        FRAMEWORK DESTINATION ${FOUNDATION_FRAMEWORK_INSTALL_DIR}
    )
else()
endif()


add_library(${FOUNDATION_SHARED_LIBRARY_NAME} SHARED ${FOUNDATION_SOURCE_FILES})
set_target_properties(${FOUNDATION_SHARED_LIBRARY_NAME} PROPERTIES
    OUTPUT_NAME ${FOUNDATION_LIBRARY_NAME}
)
if(FOUNDATION_SHARED_LIBRARY_COMPILE_FLAGS})
    target_compile_options(${FOUNDATION_SHARED_LIBRARY_NAME} PRIVATE
        ${FOUNDATION_SHARED_LIBRARY_COMPILE_FLAGS}
    )
endif()

add_library(${FOUNDATION_STATIC_LIBRARY_NAME} STATIC ${FOUNDATION_SOURCE_FILES)
set_target_properties(${FOUNDATION_STATIC_LIBRARY_NAME} PROPERTIES
    OUTPUT_NAME ${FOUNDATION_LIBRARY_NAME)
if(FOUNDATION_STATIC_LIBRARY_COMPILE_FLAGS)
    target_compile_options(${FOUNDATION_STATIC_LIBRARY_NAME PRIVATE
        ${FOUNDATION_STATIC_LIBRARY_COMPILE_FLAGS})
endif()

install(TARGETS ${FOUNDATION_SHARED_LIBRARY_NAME}
    ${FOUNDATION_STATIC_LIBRARY_NAME}
    EXPORT ${FOUNDATION_LIBRARY_NAME}
    RUNTIME DESTINATION ${FOUNDATION_RUNTIME_INSTALL_DIR}
    LIBRARY DESTINATION ${FOUNDATION_LIBRARY_INSTALL_DIR}
    ARCHIVE DESTINATION ${FOUNDATION_ARCHIVE_INSTALL_DIR}
)

install(FILES ${FOUNDATION_HEADER_FILES} DESTINATION
    include/${FOUNDATION_LIBRARY_NAME}
    )

install(EXPORT ${FOUNDATION_LIBRARY_NAME} DESTINATION
    "${FOUNDATION_BUILD_DIR}"
    )