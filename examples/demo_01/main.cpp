#include "app.hpp"
#include "database.hpp"

#include <malloy/server/routing_context.hpp>
#include <malloy/server/routing/router.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>

[[nodiscard]]
static
std::shared_ptr<spdlog::logger>
create_example_logger()
{
    auto log_level = spdlog::level::debug;

    // Sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(log_level);

    // Create logger
    auto logger = std::make_shared<spdlog::logger>("", spdlog::sinks_init_list{ console_sink });
    logger->set_level(log_level);

    return logger;
}

int main()
{
    using namespace malloy;

    // Create malloy controller config
    malloy::server::routing_context::config cfg;
    cfg.interface   = "127.0.0.1";
    cfg.port        = 8080;
    cfg.doc_root    = "./";
    cfg.num_threads = 1;
    cfg.logger      = create_example_logger();

    // Create malloy controller
    malloy::server::routing_context c{cfg};

    // Setup the database
    auto db = std::make_shared<database>(cfg.logger->clone("database"));
    if (!db->init()) {
        cfg.logger->critical("could not initialize database.");
        return EXIT_FAILURE;
    }

    // Setup application environment
    zim::environment env {
        .site {
            .base_url       = "http://127.0.0.1:8080",
        },
        .app {
            .base_url       = "http://127.0.0.1:8080",
            .assets_fs_path = "../../../examples/demo_01/assets",
        }
    };

    // Create top-level application
    auto toplevel_app = std::make_shared<app>(
        cfg.logger->clone("app"),
        env,
        db
    );
    if (!toplevel_app->init()) {
        cfg.logger->critical("initializing top-level app failed.");
        return EXIT_FAILURE;
    }

    // Add top-level app router
    c.router().add_subrouter("/apps", std::move(toplevel_app->router()));

    // Start
    start(std::move(c)).run();

    return EXIT_SUCCESS;
}
