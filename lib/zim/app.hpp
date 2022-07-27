#pragma once

#include "environment.hpp"
#include "database/types.hpp"

#include <malloy/server/routing/router.hpp>
#include <spdlog/logger.h>

#include <concepts>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace zim
{
    class controller;   // ToDo: See friend declaration below
    class page;

    /**
     * A base class for an application.
     */
    class app
    {
        // ToDo: only until the controller provides a root-app
        friend controller;

    public:
        app(
            std::shared_ptr<spdlog::logger> logger,
            std::string name,
            environment env
        );

        app() = default;
        app(const app&) = delete;
        app(app&&) noexcept = delete;
        virtual ~app() noexcept = default;

        app& operator=(const app&) = delete;
        app& operator=(app&&) noexcept = delete;

        [[nodiscard]]
        virtual
        bool
        init() = 0;

        [[nodiscard]]
        std::string_view
        name() const noexcept
        {
            return m_name;
        }

        [[nodiscard]]
        std::unique_ptr<malloy::server::router>
        router() noexcept
        {
            return std::move(m_router);
        }

    protected:
        environment m_env;
        std::shared_ptr<spdlog::logger> m_logger;
        std::unique_ptr<malloy::server::router> m_router;

        /**
         * Adds an endpoint for an HTML page.
         *
         * @param target The target path.
         * @param page The HTML page.
         * @return Whether adding the endpoint was successful.
         */
        bool
        add_page(std::string&& resource, std::shared_ptr<page>);

        /**
         * Makes a new sub-application of type @ref App.
         *
         * @tparam App The app type.
         * @tparam Args `App` constructor type list.
         * @param name The name of the new sub-app.
         * @param args The arguments passed to the contructor of the new sub-app.
         * @return Whether making the sub-app was successful.
         */
        template<class App, typename... Args>
            requires std::derived_from<App, app>
        bool
        make_subapp(const std::string& name, Args&&... args)
        {
            // Sanity check name
            if (name.empty()) {
                m_logger->error("could not make sub-app. name must not be empty.");
                return false;
            }

            // Make sure that the parent app (this) still has a router
            if (!m_router) {
                m_logger->error("could not make sub-app. parent app has no router.");
                return false;
            }

            // Create & setup app
            auto app = std::make_shared<App>(std::forward<Args>(args)...);
            app->m_name = name;
            app->m_env = m_env.make_sub_environment(name);
            app->m_logger = m_logger->clone(m_logger->name() + " | " + name);
            app->m_router = std::make_unique<malloy::server::router>(m_logger->clone(m_logger->name() + " | "  + name + " | router"));
            app->m_db_session_getter = m_db_session_getter; // ToDo: Is this thread safe when we copy it?

            // Initialize app
            if (!app->init()) {
                m_logger->error("initialization of sub-app \"{}\" failed.", name);
                return false;
            }

            // Add the sub-router
            const std::string target_base = "/" + name;
            if (!m_router->add_subrouter(target_base, app->router())) {
                m_logger->error("could not add router of sub-app");
                return false;
            }

            // Housekeeping
            m_subapps.emplace_back(std::move(app));

            return true;
        }

        /**
         * Get a copy of the database session getter.
         */
        [[nodiscard]]
        database::session_getter_t
        database_session_getter() const
        {
            return m_db_session_getter;
        }

        /**
         * Get a database session.
         *
         * @return The database session.
         */
        [[nodiscard]]
        std::shared_ptr<database::session>
        get_database_session() const
        {
            if (!m_db_session_getter)
                return { };

            return m_db_session_getter();
        }

    private:
        std::string m_name;
        std::vector<std::shared_ptr<app>> m_subapps;
        database::session_getter_t m_db_session_getter;
    };

}
