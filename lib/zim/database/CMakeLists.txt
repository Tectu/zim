target_sources(
    ${TARGET}
    PUBLIC
        FILE_SET
        HEADERS
        BASE_DIRS ${ZIM_HEADERS_BASE_DIR}
        FILES
            client.hpp
            config.hpp
            manager.hpp
            session.hpp
            types.hpp

    PRIVATE
        manager.cpp
)
