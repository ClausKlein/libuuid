cmake_minimum_required(VERSION 3.25...4.1)

project(libuuid VERSION 1.0.3 LANGUAGES C)

set(CMAKE_SKIP_TEST_ALL_DEPENDENCY OFF)

add_definitions(-DHAVE_CONFIG_H)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(
    uuid
    STATIC
    sysdep.c
    uuid.c
    copyrt.h
    sysdep.h
    uuid.h
    md5.h
    md5c.c
    global.h
)
target_include_directories(uuid PUBLIC "$<INSTALL_INTERFACE:include>")

enable_testing()
add_executable(test_md5 mddriver.c)
target_link_libraries(test_md5 uuid)
add_test(NAME test_md5-x COMMAND test_md5 -x)
add_test(NAME test_md5-t COMMAND test_md5 -t)
add_test(NAME test_md5 COMMAND test_md5 CMakeCache.txt)

add_executable(test_uuid utest.c)
target_link_libraries(test_uuid uuid)
add_test(NAME test_uuid COMMAND test_uuid)

if(CMAKE_SKIP_INSTALL_RULES)
    return()
endif()

install(FILES uuid.h copyrt.h DESTINATION include/uuid)

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
