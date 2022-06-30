# Find SQLite3
find_package(
    SQLite3
    REQUIRED
)

# Set a target name
set(TARGET zim-demo01)

# Create the executable
add_executable(${TARGET})

add_subdirectory(apps)

target_sources(
    ${TARGET}
    PRIVATE
        main.cpp
        app.cpp
        app.hpp
        form_renderer.cpp
        form_renderer.hpp
        database.cpp
        database.hpp
        page_master.hpp
)

target_compile_features(
    ${TARGET}
    PRIVATE
        cxx_std_20
)

target_link_libraries(
    ${TARGET}
    PRIVATE
        zim
        SQLite::SQLite3
)
