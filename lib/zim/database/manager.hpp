#pragma once

#include "config.hpp"
#include "session.hpp"

#include <memory>

namespace soci
{
    class connection_pool;
}

namespace zim::database
{

    /**
     * The database manager.
     *
     * @note A user of the `zim` library does not typically interact with this class directly.
     */
    // ToDo: Long term: Allow interfacing multiple databases.
    class manager
    {
    public:
        /**
         * Constructor.
         *
         * @param cfg The configuration.
         */
        explicit
        manager(const config& cfg);

        /**
         * Destructor.
         */
        virtual
        ~manager() noexcept;

        /**
         * @note This will block until a session becomes available from the session pool.
         * @note The session is automatically returned to the pool once it gets destructed.
         *
         * @return The session.
         */
        // ToDo: Add timeout parameter
        [[nodiscard]]
        std::shared_ptr<session>
        get_session() const;

    private:
        soci::connection_pool* m_pool = nullptr;
    };

}
