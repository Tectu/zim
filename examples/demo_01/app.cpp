#include "app.hpp"
#include "database.hpp"
#include "page_master.hpp"
#include "apps/gallery/app.hpp"

#include <malloy/server/routing/router.hpp>
#include <spdlog/logger.h>

app::app(
    std::shared_ptr<spdlog::logger> logger,
    std::shared_ptr<database> db
) :
    m_db(std::move(db))
{
    // Sanity check
    if (!m_db)
        throw std::invalid_argument("no valid database provided.");
}

bool
app::init()
{
    // Set database session getter
    m_db->session_getter = database_session_getter();

    // Create master page
    m_master_page = std::make_shared<page_master>();

    // Create sub-apps
    make_subapp<apps::gallery::app>("gallery", m_db, m_master_page);

    // Endpoints
    //m_router->add_file_serving("/assets", m_env.app.assets_fs_path);
    m_router->add_file_serving("/assets", "../../../examples/demo_01/assets");  // ToDo: Use environment

    return true;
}
