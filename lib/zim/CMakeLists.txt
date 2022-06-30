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
            environment.hpp
            page.hpp
            page_content.hpp
            page_master.hpp

    PRIVATE
        app.cpp
        page_content.cpp
        page_master.cpp
)

target_link_libraries(
    ${TARGET}
    PUBLIC
        malloy::malloy-server
)
