include(external.cmake)

set(TARGET zim)

add_library(${TARGET})

target_compile_features(
    ${TARGET}
    PUBLIC
        cxx_std_20
)

target_include_directories(
    ${TARGET}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>
)

target_sources(
    ${TARGET}
    PUBLIC
        FILE_SET
        HEADERS
        FILES
            app.hpp
            controller.hpp
            environment.hpp
            page.hpp
            page_content.hpp
            page_master.hpp

    PRIVATE
        app.cpp
        controller.cpp
        page_content.cpp
        page_master.cpp
)

target_link_libraries(
    ${TARGET}
    PUBLIC
        malloy::malloy-server
        soci_core
        soci_sqlite3
)


########################################################################################################################
# Install                                                                                                              #
########################################################################################################################

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)


write_basic_package_version_file(
    zim-config-version.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_file(zim-config.cmake.in zim-config.cmake @ONLY)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/zim-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/zim-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zim
)

# Targets
install(
    TARGETS
        ${TARGET}
    EXPORT zim-targets
    FILE_SET
        HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zim
)

install(
    EXPORT zim-targets
    FILE zim-targets.cmake
    NAMESPACE zim::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zim
)