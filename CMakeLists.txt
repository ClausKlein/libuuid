cmake_minimum_required(VERSION 3.21...4.1)

project(libuuid VERSION 1.0.3 LANGUAGES C)

if(UNIX)
    configure_file(config.Unix.h config.h COPYONLY)
else()
    configure_file(config.Windows.h config.h COPYONLY)
endif()

add_definitions(-DHAVE_CONFIG_H)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(
    uuid
    STATIC
    clear.c
    compare.c
    copy.c
    gen_uuid.c
    isnull.c
    pack.c
    parse.c
    randutils.c
    unpack.c
    unparse.c
    uuid_time.c
)
target_include_directories(uuid PUBLIC "$<INSTALL_INTERFACE:include>")

enable_testing()
add_executable(test_uuid test_uuid.c)
target_link_libraries(test_uuid uuid)
add_test(NAME test_uuid COMMAND test_uuid)

if(CMAKE_SKIP_INSTALL_RULES)
    return()
endif()

install(FILES uuid.h DESTINATION include/uuid)

install(
    TARGETS uuid
    EXPORT uuid_targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
set(PACKAGE_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/libuuid-config.cmake")
set(INSTALL_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/libuuid")

write_basic_package_version_file(
    "libuuid-config-version.cmake"
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    libuuid-config.cmake.in
    "${PACKAGE_CONFIG_FILE}"
    INSTALL_DESTINATION "${INSTALL_CONFIG_DIR}"
)

export(
    EXPORT uuid_targets
    NAMESPACE UUID::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/libuuid-targets.cmake"
)

install(
    EXPORT uuid_targets
    NAMESPACE UUID::
    FILE libuuid-targets.cmake
    DESTINATION "${INSTALL_CONFIG_DIR}"
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/libuuid-config-version.cmake"
        "${PACKAGE_CONFIG_FILE}"
    DESTINATION "${INSTALL_CONFIG_DIR}"
)

if(PROJECT_IS_TOP_LEVEL)
    set(CPACK_GENERATOR TGZ)
    include(CPack)
endif()
