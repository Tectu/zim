include(FetchContent)


########################################################################################################################
# malloy: https://github.com/tectu/malloy
########################################################################################################################
FetchContent_Declare(
    malloy
    GIT_REPOSITORY https://github.com/tectu/malloy
    GIT_TAG        main
)
FetchContent_GetProperties(malloy)
if(NOT malloy_POPULATED)
    FetchContent_Populate(malloy)
    set(MALLOY_BUILD_EXAMPLES OFF CACHE INTERNAL "")
    set(MALLOY_BUILD_TESTS OFF CACHE INTERNAL "")
    set(MALLOY_BUILD_SHARED OFF CACHE INTERNAL "")
    set(MALLOY_FEATURE_CLIENT OFF CACHE INTERNAL "")
    set(MALLOY_FEATURE_SERVER ON CACHE INTERNAL "")
    set(MALLOY_FEATURE_HTML ON CACHE INTERNAL "")
    set(MALLOY_FEATURE_TLS ${ELX_FEATURE_TLS} CACHE INTERNAL "")
    set(MALLOY_DEPENDENCY_FMT_DOWNLOAD OFF CACHE INTERNAL "")
    set(MALLOY_DEPENDENCY_SPDLOG_DOWNLOAD OFF CACHE INTERNAL "")

    add_subdirectory(${malloy_SOURCE_DIR} ${malloy_BINARY_DIR} EXCLUDE_FROM_ALL)    # EXCLUDE_FROM_ALL to prevent install()

    add_library(malloy::malloy-server ALIAS malloy-server)      # ToDo: Should this be part of malloy itself?
endif()


########################################################################################################################
# SOCI: https://github.com/SOCI/soci
########################################################################################################################
FetchContent_Declare(
    soci
    GIT_REPOSITORY https://github.com/SOCI/soci
    GIT_TAG        master
)
FetchContent_GetProperties(soci)
if (NOT soci_POPULATED)
    FetchContent_Populate(soci)

    set(SOCI_SHARED ON  CACHE INTERNAL "")
    set(SOCI_STATIC OFF  CACHE INTERNAL "")
    set(SOCI_TESTS  OFF CACHE INTERNAL "")
    set(SOCI_VISIBILITY OFF CACHE INTERNAL "")

    add_subdirectory(${soci_SOURCE_DIR} ${soci_BINARY_DIR})
endif()
