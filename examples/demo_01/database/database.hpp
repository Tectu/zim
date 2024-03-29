#pragma once

#include "../types/image.hpp"

#include <zim/database/client.hpp>

#include <memory>
#include <optional>
#include <string>

namespace soci
{
    class session;
}

namespace spdlog
{
    class logger;
}

/**
 * Database class.
 *
 * This is a VERY basic implementation. It was kept to a bare minimum for demonstration purposes.
 * It doesn't do any SQL injection protections/checks or other very important security checks.
 */
class database :
    public zim::database::client
{
public:
    explicit database(std::shared_ptr<spdlog::logger> logger);
    database(const database&) = delete;
    database(database&&) noexcept = delete;
    virtual ~database() noexcept;

    database& operator=(const database&) = delete;
    database& operator=(database&&) noexcept = delete;

    bool
    create_tables();

    bool
    add_image(const image& img);

    std::vector<image>
    images();

    std::optional<image>
    get_image(int id);

private:
    std::shared_ptr<spdlog::logger> m_logger;
};
