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
database::create_tables()
{
    auto session = get_session();
    if (!session)
        return false;

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
        soci::statement st = (session->prepare << stmt);

        st.execute(true);
    }

    return true;
}

bool
database::add_image(const image& img)
{
    auto session = get_session();
    if (!session)
        return false;

    // ToDo: error handling
    soci::statement stmt = (session->prepare << "INSERT INTO images (caption, data) VALUES(:1, :2)", soci::use(img.caption), soci::use(img.data));

    // ToDo:: error handling
    stmt.execute(true);

    return true;
}

std::vector<image>
database::images()
{
    auto session = get_session();
    if (!session)
        return { };

    std::vector<image> images;

    // ToDo: Error handling
    soci::rowset<soci::row> rs = (session->prepare << "SELECT id, caption, data FROM IMAGES");
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

std::optional<image>
database::get_image(const int id)
{
    auto session = get_session();
    if (!session)
        return { };

    // ToDo: Error handling
    soci::rowset<soci::row> rs = (session->prepare << "SELECT id, caption, data FROM IMAGES WHERE id = :1", soci::use(id));
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
