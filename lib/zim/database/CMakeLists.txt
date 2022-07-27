target_sources(
    ${TARGET}
    PUBLIC
        FILE_SET
        HEADERS
        BASE_DIRS ${ZIM_HEADERS_BASE_DIR}
        FILES
            manager.hpp
            session.hpp
            types.hpp

    PRIVATE
        manager.cpp
)
