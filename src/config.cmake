################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(src/alarm/config.cmake)
include(src/allocator/config.cmake)
include(src/array/config.cmake)
include(src/base64/config.cmake)
include(src/commandline/config.cmake)
include(src/copy/config.cmake)
include(src/numerics/config.cmake)
include(src/data/config.cmake)
include(src/date/config.cmake)
include(src/defer/config.cmake)
include(src/dll/config.cmake)
include(src/encoding/config.cmake)
include(src/environment/config.cmake)
include(src/files/config.cmake)
include(src/format/config.cmake)
include(src/include/config.cmake)
include(src/log/config.cmake)
include(src/multivalue/config.cmake)
include(src/network/config.cmake)
include(src/notification/config.cmake)
include(src/pipe/config.cmake)
include(src/platform/config.cmake)
include(src/poll/config.cmake)
include(src/process/config.cmake)
include(src/progress/config.cmake)
include(src/queue/config.cmake)
include(src/random/config.cmake)
if (UNIX)
    include(src/signal/config.cmake)
endif()
include(src/sleep/config.cmake)
include(src/stl/config.cmake)
include(src/string/config.cmake)
include(src/symbol/config.cmake)
include(src/template/config.cmake)
include(src/thread/config.cmake)
include(src/user/config.cmake)
include(src/uuid/config.cmake)
include(src/version/config.cmake)

include_directories(
    src/alarm
    src/allocator
    src/array
    src/base64
    src/commandline
    src/copy
    src/data
    src/date
    src/defer
    src/dll
    src/encoding
    src/environment
    src/files
    src/format
    src/include
    src/log
    src/multivalue
    src/network
    src/notification
    src/numerics
    src/pipe
    src/platform
    src/poll
    src/process
    src/progress
    src/queue
    src/random
    src/sleep
    src/stl
    src/string
    src/symbol
    src/template
    src/thread
    src/user
    src/uuid
    src/version
    "${GENERATED_SOURCES_DIR}"
)

if(UNIX)
    include_directories(
        src/dll/unix
        src/files/unix
        src/network/unix
        src/process/unix
        src/signal
        src/uuid/unix
    )
endif()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/headers/${FOUNDATION_LIBRARY_NAME})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/headers/${FOUNDATION_LIBRARY_NAME}/updated
	COMMAND cp ${FOUNDATION_HEADER_FILES} ${CMAKE_CURRENT_BINARY_DIR}/headers/${FOUNDATION_LIBRARY_NAME}
	COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/headers/${FOUNDATION_LIBRARY_NAME}/updated
	DEPENDS ${FOUNDATION_HEADER_FILES}
	COMMENT "Collecting Foundation headers"
)

add_custom_target(FoundationHeaders DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/headers/${FOUNDATION_LIBRARY_NAME}/updated)


if(APPLE)
    add_library(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} SHARED ${FOUNDATION_SOURCE_FILES})
    set_target_properties(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} PROPERTIES
        FRAMEWORK ON
        OUTPUT_NAME ${FOUNDATION_LIBRARY_NAME}
        PUBLIC_HEADER "${FOUNDATION_HEADER_FILES}"
        MACOSX_RPATH TRUE
    )
    if(FOUNDATION_FRAMEWORK_LIBRARY_COMPILE_FLAGS)
        target_compile_options(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} PRIVATE
            ${FOUNDATION_FRAMEWORK_LIBRARY_COMPILE_FLAGS}
        )
    endif()
    if(FOUNDATION_FRAMEWORK_LIBRARY_COMPILE_DEFINITIONS)
        target_compile_definitions(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} PRIVATE
            ${FOUNDATION_FRAMEWORK_COMPILE_DEFINITIONS})
    endif()
    if(FOUNDATION_FRAMEWORK_LIBRARY_LINK_FLAGS)
        set_target_properties(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} PROPERTIES
            LINK_FLAGS "${FOUNDATION_FRAMEWORK_LIBRARY_LINK_FLAGS}")
    endif()
    if(FOUNDATION_FRAMEWORK_LIBRARY_LIBRARIES)
        # As of the time of this writing on macOS, we need to have both a PRIVATE and INTERFACE
        # call for target_link_libraries.
        target_link_libraries(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} PRIVATE
            ${FOUNDATION_FRAMEWORK_LIBRARY_LIBRARIES})
        target_link_libraries(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} INTERFACE
            ${FOUNDATION_FRAMEWORK_LIBRARY_LIBRARIES})
    endif()
    target_include_directories(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} INTERFACE
        $<INSTALL_INTERFACE:include/TFFoundation>)
    add_dependencies(${FOUNDATION_FRAMEWORK_LIBRARY_NAME} FoundationHeaders)

    install(TARGETS ${FOUNDATION_FRAMEWORK_LIBRARY_NAME}
        EXPORT ${FOUNDATION_LIBRARY_NAME}
        FRAMEWORK DESTINATION ${FOUNDATION_FRAMEWORK_INSTALL_DIR}
    )
endif()


add_library(${FOUNDATION_SHARED_LIBRARY_NAME} SHARED ${FOUNDATION_SOURCE_FILES})
set_target_properties(${FOUNDATION_SHARED_LIBRARY_NAME} PROPERTIES
    OUTPUT_NAME ${FOUNDATION_LIBRARY_NAME}
)
if(APPLE)
    set_target_properties(${FOUNDATION_SHARED_LIBRARY_NAME} PROPERTIES
        MACOSX_RPATH TRUE)
endif()
if(FOUNDATION_SHARED_LIBRARY_COMPILE_DEFINITIONS)
    target_compile_definitions(${FOUNDATION_SHARED_LIBRARY_NAME} PUBLIC
        ${FOUNDATION_SHARED_LIBRARY_COMPILE_DEFINITIONS})
endif()
if(FOUNDATION_SHARED_LIBRARY_COMPILE_FLAGS)
    target_compile_options(${FOUNDATION_SHARED_LIBRARY_NAME} PRIVATE
        ${FOUNDATION_SHARED_LIBRARY_COMPILE_FLAGS}
    )
endif()
if(FOUNDATION_SHARED_LIBRARY_LINK_FLAGS)
    set_target_properties(${FOUNDATION_SHARED_LIBRARY_NAME} PROPERTIES
        LINK_FLAGS "${FOUNDATION_SHARED_LIBRARY_LINK_FLAGS}")
endif()
if(FOUNDATION_SHARED_LIBRARY_LIBRARIES)
    # As of the time of this writing on macOS, we need to have both a PRIVATE and INTERFACE
    # call for target_link_libraries.
    target_link_libraries(${FOUNDATION_SHARED_LIBRARY_NAME} PRIVATE
        ${FOUNDATION_SHARED_LIBRARY_LIBRARIES})
    target_link_libraries(${FOUNDATION_SHARED_LIBRARY_NAME} INTERFACE
        ${FOUNDATION_SHARED_LIBRARY_LIBRARIES})
endif()
target_include_directories(${FOUNDATION_SHARED_LIBRARY_NAME} INTERFACE
    $<INSTALL_INTERFACE:include/TFFoundation>)
add_dependencies(${FOUNDATION_SHARED_LIBRARY_NAME} FoundationHeaders)


add_library(${FOUNDATION_STATIC_LIBRARY_NAME} STATIC ${FOUNDATION_SOURCE_FILES})
set_target_properties(${FOUNDATION_STATIC_LIBRARY_NAME} PROPERTIES
    OUTPUT_NAME ${FOUNDATION_LIBRARY_NAME})
if(FOUNDATION_STATIC_LIBRARY_COMPILE_DEFINITIONS)
    target_compile_definitions(${FOUNDATION_STATIC_LIBRARY_NAME} PUBLIC
        ${FOUNDATION_STATIC_LIBRARY_COMPILE_DEFINITIONS})
endif()
if(FOUNDATION_STATIC_LIBRARY_COMPILE_FLAGS)
    target_compile_options(${FOUNDATION_STATIC_LIBRARY_NAME} PRIVATE
        ${FOUNDATION_STATIC_LIBRARY_COMPILE_FLAGS})
endif()
if(FOUNDATION_STATIC_LIBRARY_LINK_FLAGS)
    set_target_properties(${FOUNDATION_STATIC_LIBRARY_NAME} PROPERTIES
        LINK_FLAGS "${FOUNDATION_STATIC_LIBRARY_LINK_FLAGS}")
endif()
if (FOUNDATION_STATIC_LIBRARY_LIBRARIES)
    target_link_libraries(${FOUNDATION_STATIC_LIBRARY_NAME} INTERFACE
        ${FOUNDATION_STATIC_LIBRARY_LIBRARIES})
endif()
target_include_directories(${FOUNDATION_STATIC_LIBRARY_NAME} INTERFACE
    $<INSTALL_INTERFACE:include/TFFoundation>)
add_dependencies(${FOUNDATION_STATIC_LIBRARY_NAME} FoundationHeaders)


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

#install(EXPORT ${FOUNDATION_LIBRARY_NAME} DESTINATION
#    "${FOUNDATION_BUILD_DIR}"
#    )

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TFFoundation/TFFoundationConfigVersion.cmake"
    VERSION ${TFFoundation_VERSION}
    COMPATIBILITY AnyNewerVersion
)

export(EXPORT ${FOUNDATION_LIBRARY_NAME}
    FILE "${CMAKE_CURRENT_BINARY_DIR}/TFFoundation/TFFoundationTargets.cmake"
    NAMESPACE TFFoundation::
)

configure_file(cmake/TFFoundationConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/TFFoundation/TFFoundationConfig.cmake"
    COPYONLY)

set(ConfigPackageLocation lib/cmake/TFFoundation)
install(EXPORT ${FOUNDATION_LIBRARY_NAME}
    FILE TFFoundationTargets.cmake
    NAMESPACE TFFoundation::
    DESTINATION ${ConfigPackageLocation})
install(FILES
    cmake/TFFoundationConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/TFFoundation/TFFoundationConfigVersion.cmake"
    DESTINATION ${ConfigPackageLocation})
