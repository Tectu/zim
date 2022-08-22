target_sources(
    ${TARGET}
    PUBLIC
        FILE_SET
        HEADERS
        BASE_DIRS ${ZIM_HEADERS_BASE_DIR}
        FILES
            content.hpp
            form.hpp
            forms.hpp
            master.hpp
            page.hpp

    PRIVATE
        content.cpp
        form.cpp
        forms.cpp
        master.cpp
)
