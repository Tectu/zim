#pragma once

#include "app.hpp"
#include "database/client.hpp"
#include "database/config.hpp"

#include <malloy/server/routing_context.hpp>

#include <concepts>
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

namespace zim::database
{
    class manager;
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

            /**
             * Database related configuration.
             */
            database::config database;

            struct site {
                std::string base_url = "http://127.0.0.1:8080";
            } site;
        };

        controller() = default;
        controller(const controller&) = delete;
        controller(controller&&) noexcept = delete;
        virtual ~controller() noexcept = default;

        controller& operator=(const controller&) = delete;
        controller& operator=(controller&&) noexcept = delete;

        [[nodiscard("init might fail")]]
        bool
        init(config&& cfg);

        void
        start();

        void
        stop();

        [[nodiscard]]
        std::optional<database::client>
        make_database_client();

        template<class App, typename... Args>
            requires std::derived_from<App, app>
        bool
        make_app(const std::string& name, Args&&... args)
        {
            // Sanity check
            if (!m_toplevel_app)
                return false;

            // Delegate
            return m_toplevel_app->make_subapp<App, Args...>(name, std::forward<Args>(args)...);
        }

    private:
        config m_cfg;
        std::shared_ptr<spdlog::logger> m_logger;
        std::shared_ptr<malloy::server::routing_context> m_malloy_controller;
        std::optional<malloy::server::routing_context::session> m_malloy_session;
        std::shared_ptr<database::manager> m_db_manager;
        std::shared_ptr<zim::app> m_toplevel_app;

        bool
        create_toplevel_app();
    };

}
