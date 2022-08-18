#include "app.hpp"
#include "database/database.hpp"
#include "../example.hpp"

#include <zim/controller.hpp>

#include <iostream>

int main()
{
    auto logger = example::create_logger();

    // Setup the database
    auto db = std::make_shared<database>(logger->clone("database"));

    // Controller config
    zim::controller::config cfg {
        .logger = logger,

        .database = {
            .type = "sqlite3",
            .connection_string = "demo_01.sqlite",
        },

        .site = {
            .base_url = "http://127.0.0.1:8080",
        },
    };

    // Controller
    zim::controller c;
    if (!c.init(std::move(cfg))) {
        logger->critical("initializing zim controller failed.");
        return EXIT_FAILURE;
    }

    // Create our application
    if (!c.make_app<app>("app", db)) {
        logger->critical("could not create app.");
        return EXIT_FAILURE;
    }

    // Create database tables
    if (!db->create_tables()) {
        logger->critical("could not create database tables.");
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
