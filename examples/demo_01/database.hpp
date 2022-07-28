#pragma once

#include <zim/database/types.hpp>

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
class database
{
public:
    // ToDo: This should be more restricted. Should probably also be atomic.
    zim::database::session_getter_t session_getter;

    struct image
    {
        int id;
        std::string caption;
        std::string mime_type;
        std::string data;
    };

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
    std::shared_ptr<soci::session> m_db = nullptr;

    [[nodiscard]]
    std::shared_ptr<zim::database::session>
    get_session() noexcept;
};
