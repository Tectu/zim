cmake_minimum_required(VERSION 3.23)

# Project
project(
    zim
    VERSION 0.0.0
    LANGUAGES
        CXX
)

# Options
option(ZIM_BUILD_EXAMPLES "Whether to build examples" ON)

# Add the library
add_subdirectory(lib)

# Add examples (if supposed to)
if (ZIM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
