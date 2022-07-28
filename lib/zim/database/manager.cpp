#include "manager.hpp"

#include <soci/soci.h>

using namespace zim::database;

manager::manager(const config& cfg)
{
    // Create connection pool
    m_pool = new soci::connection_pool(cfg.connection_pool_size);

    // Populate/prepare/open each session in the pool
    // ToDo: Error handling
    for (std::size_t i = 0; i < cfg.connection_pool_size; ++i) {
        soci::session& ses = m_pool->at(i);
        ses.open(cfg.type, cfg.connection_string);
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
