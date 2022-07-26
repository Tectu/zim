include(FetchContent)


########################################################################################################################
# malloy: https://github.com/tectu/malloy
########################################################################################################################
find_package(
    malloy
    REQUIRED
)

########################################################################################################################
# nlohmann/json: https://github.com/nlohmann/json
########################################################################################################################
find_package(
    nlohmann_json
    REQUIRED
)

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
