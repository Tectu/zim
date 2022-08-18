#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace example
{

    [[nodiscard]]
    static
    inline
    std::shared_ptr<spdlog::logger>
    create_logger()
    {
        auto log_level = spdlog::level::trace;

        // Sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(log_level);

        // Create logger
        auto logger = std::make_shared<spdlog::logger>("", spdlog::sinks_init_list{ console_sink });
        logger->set_level(log_level);

        return logger;
    }

}
