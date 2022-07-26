#pragma once

#include <malloy/server/routing_context.hpp>

#include <memory>
#include <optional>

namespace spdlog
{
    class logger;
}

namespace zim
{
    class app;
}

namespace zim
{

    class controller
    {
    public:
        struct config
        {
            /**
             * The logger.
             */
            std::shared_ptr<spdlog::logger> logger;

            /**
             * Webserver related configuration.
             */
            struct webserver {
                std::string interface           = "127.0.0.1";
                uint16_t port                   = 8080;
                std::filesystem::path doc_root  = "./";
                std::size_t num_threads         = 1;
            } webserver;
        };

        controller() = default;
        controller(const controller&) = delete;
        controller(controller&&) noexcept = delete;
        virtual ~controller() noexcept = default;

        controller& operator=(const controller&) = delete;
        controller& operator=(controller&&) noexcept = delete;

        [[nodiscard("init might fail")]]
        bool
        init(config&& cfg, std::shared_ptr<zim::app> app);

        void
        start();

        void
        stop();

    private:
        config m_cfg;
        std::shared_ptr<spdlog::logger> m_logger;
        std::shared_ptr<malloy::server::routing_context> m_malloy_controller;
        std::optional<malloy::server::routing_context::session> m_malloy_session;
    };

}
