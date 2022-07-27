#include "app.hpp"
#include "database.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <zim/controller.hpp>

#include <iostream>

[[nodiscard]]
static
std::shared_ptr<spdlog::logger>
create_example_logger()
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

int main()
{
    auto logger = create_example_logger();

    // Setup the database
    auto db = std::make_shared<database>(logger->clone("database"));
    if (!db->init()) {
        logger->critical("could not initialize database.");
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
        logger->clone("app"),
        env,
        db
    );
    if (!toplevel_app->init()) {
        logger->critical("initializing top-level app failed.");
        return EXIT_FAILURE;
    }

    // Controller config
    zim::controller::config cfg {
        .logger = logger,
    };

    // Controller
    zim::controller c;
    if (!c.init(std::move(cfg), toplevel_app)) {
        logger->critical("initializing zim controller failed.");
        return EXIT_FAILURE;
    }

    // Start
    c.start();

    // Keep the application alive
    while (true) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    return EXIT_SUCCESS;
}
