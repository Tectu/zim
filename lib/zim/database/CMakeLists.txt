target_sources(
    ${TARGET}
    PUBLIC
        FILE_SET
        HEADERS
        BASE_DIRS ${ZIM_HEADERS_BASE_DIR}
        FILES
            manager.hpp
            session.hpp

    PRIVATE
        manager.cpp
)
