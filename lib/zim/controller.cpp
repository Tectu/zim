#include "controller.hpp"
#include "app.hpp"

#include <spdlog/logger.h>

using namespace zim;

bool
controller::init(config&& cfg, std::shared_ptr<zim::app> app)
{
    // Sanity check
    if (!cfg.logger)
        return false;

    cfg.logger->trace("controller::init()");

    // Sanity check
    if (!app) {
        cfg.logger->critical("no valid top-level app provided.");
        return false;
    }

    // Grab config
    m_cfg = std::move(cfg);

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

    // Register top-level app
    m_malloy_controller->router().add_subrouter("/apps", std::move(app->router()));

    return true;
}

void
controller::start()
{
    m_cfg.logger->trace("controller::start()");

    // Start malloy
    m_cfg.logger->debug("starting malloy...");
    m_malloy_session.emplace( std::move(malloy::server::start(std::move(*m_malloy_controller))) );
}

void
controller::stop()
{
    m_cfg.logger->trace("controller::stop()");

    // Stop malloy
    m_cfg.logger->debug("stopping malloy...");
    m_malloy_session = std::nullopt;
}
