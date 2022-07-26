#include "database.hpp"

#include <soci/soci.h>
#include <spdlog/logger.h>

#include <stdexcept>

database::database(std::shared_ptr<spdlog::logger> logger) :
    m_logger(std::move(logger))
{
    if (!m_logger)
        throw std::invalid_argument("no valid logger provided.");
}

database::~database() noexcept
{
    // ToDo: Anything we have to do with the SOCI session?
}

bool
database::init()
{
    // Connect to database
    try {
        m_db = std::make_shared<soci::session>("sqlite3", "demo_01.sqlite");
    }
    catch (const std::exception& e) {
        m_logger->critical("could not open database: {}", e.what());
        m_db = { };
        return false;
    }

    // Create tables
    {
        std::vector<std::string> statements;

        // Table "images"
        statements.emplace_back(
            "CREATE TABLE IF NOT EXISTS images"
            "("
            "  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "  caption TEXT NOT NULL,"
            "  data BLOB NOT NULL"
            ");"
        );

        // ToDo: Error checking
        for (const auto& stmt : statements) {
            soci::statement st = (m_db->prepare << stmt);

            st.execute(true);
        }
    }

    return true;
}

bool
database::add_image(const std::string& caption, const std::string& data)
{
    // Sanity check
    if (!m_db)
        return false;

    // ToDo: error handling
    soci::statement stmt = (m_db->prepare << "INSERT INTO images (caption, data) VALUES(:1, :2)", soci::use(caption), soci::use(data));

    // ToDo:: error handling
    stmt.execute(true);

    return true;
}

std::vector<database::image>
database::images()
{
    // Sanity check
    if (!m_db)
        return { };

    std::vector<image> images;

    // ToDo: Error handling
    soci::rowset<soci::row> rs = (m_db->prepare << "SELECT id, caption, data FROM IMAGES");
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
        const soci::row& row = *it;

        try {
            image img;
            img.id      = row.get<int>(0);
            img.caption = row.get<std::string>(1);
            img.data    = row.get<std::string>(2);
            images.emplace_back(std::move(img));
        }
        catch (const std::exception& e) {
            m_logger->warn("exception: {}", e.what());
            return { };
        }
    }

    return images;
}

std::optional<database::image>
database::get_image(const int id)
{
    // Sanity check
    if (!m_db)
        return { };

    soci::rowset<soci::row> rs = (m_db->prepare << "SELECT id, caption, data FROM IMAGES WHERE id = :1", soci::use(id));
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
        const soci::row& row = *it;

        try {
            image img;
            img.id      = row.get<int>(0);
            img.caption = row.get<std::string>(1);
            img.data    = row.get<std::string>(2);

            return img;
        }
        catch (const std::exception& e) {
            m_logger->warn("exception: {}", e.what());
            return { };
        }
    }

    return { };
}
