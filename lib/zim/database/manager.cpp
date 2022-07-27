#include "manager.hpp"

#include <soci/soci.h>

using namespace zim::database;

manager::manager(const std::size_t pool_size)
{
    // Create connection pool
    m_pool = new soci::connection_pool(pool_size);

    // Populate/prepare/open each session in the pool
    // ToDo: Error handling
    for (std::size_t i = 0; i < pool_size; ++i) {
        soci::session& ses = m_pool->at(i);
        ses.open("sqlite3", "demo_01.sqlite");      // ToDo
    }
}

manager::~manager() noexcept
{
    delete m_pool;
}

std::shared_ptr<session>
manager::get_session() const
{
    return std::make_shared<session>(*m_pool);
}
