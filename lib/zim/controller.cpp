#include "controller.hpp"
#include "database/manager.hpp"

#include <spdlog/logger.h>

using namespace zim;

bool
controller::init(config&& cfg)
{
    // Sanity check
    if (!cfg.logger)
        return false;

    cfg.logger->trace("controller::init()");

    // Grab config
    m_cfg = std::move(cfg);

    // Database
    {
        // Manager
        m_db_manager = std::make_shared<database::manager>(m_cfg.database);
    }

    // Malloy
    {
        // Create malloy logger
        auto malloy_logger = m_cfg.logger->clone("malloy");
        malloy_logger->set_level(spdlog::level::warn);

        // Create malloy controller config
        malloy::server::routing_context::config malloy_cfg;
        malloy_cfg.interface   = m_cfg.webserver.interface;
        malloy_cfg.port        = m_cfg.webserver.port;
        malloy_cfg.doc_root    = m_cfg.webserver.doc_root;
        malloy_cfg.num_threads = m_cfg.webserver.num_threads;
        malloy_cfg.logger      = std::move(malloy_logger);

        // Create malloy controller
        try {
            m_malloy_controller = std::make_shared<malloy::server::routing_context>(malloy_cfg);
        }
        catch (const std::exception& e) {
            m_cfg.logger->critical("could not initialize malloy controller.");
            return false;
        }
    }

    // Top-level app
    {
        // Create
        if (!create_toplevel_app()) {
            m_cfg.logger->critical("could not create top-level app");
            return false;
        }
    }

    return true;
}

// ToDo: Database manager/connections
void
controller::start()
{
    m_cfg.logger->trace("controller::start()");

    // Add the top-level router
    // Note: We do this here rather than anywhere else as this "removes" the router from the top-level app making any subsequent calls to
    //       controller::make_app() and app::make_subapp() invalid as the top-level app would not have any router anymore.
    m_malloy_controller->router().add_subrouter("/" + std::string{ m_toplevel_app->name() }, std::move(m_toplevel_app->router()));

    // Start malloy
    m_cfg.logger->debug("starting malloy...");
    m_malloy_session.emplace( std::move(malloy::server::start(std::move(*m_malloy_controller))) );
}

// ToDo: Database manager/connections
void
controller::stop()
{
    m_cfg.logger->trace("controller::stop()");

    // Stop malloy
    m_cfg.logger->debug("stopping malloy...");
    m_malloy_session = std::nullopt;
}

bool
controller::create_toplevel_app()
{
    // Setup application environment
    // ToDo: Use supplied config!
    zim::environment env {
        .site {
            .base_url       = "http://127.0.0.1:8080",
        },
        .app {
            .base_url       = "http://127.0.0.1:8080/apps",
            .assets_fs_path = "../../../examples/demo_01/assets",
        }
    };

    // Create app
    auto app = std::make_shared<zim::app>();
    app->m_name = "apps";
    app->m_env = std::move(env);
    app->m_logger = m_cfg.logger->clone(m_cfg.logger->name() + " | apps");
    app->m_router = std::make_unique<malloy::server::router>(m_cfg.logger->clone(m_cfg.logger->name() + " | apps | router"));
    app->m_db_session_getter = [this]{
        // ToDo: Check for valid database manager (and probably non-null session getter)
        return m_db_manager->get_session();
    };

    // Initialize app
    if (!app->init()) {
        m_cfg.logger->error("could not initialize top-level app");
        return false;
    }

    // Done
    m_toplevel_app = std::move(app);

    return true;
}
