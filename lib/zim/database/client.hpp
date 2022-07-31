#pragma once

#include "session.hpp"
#include "types.hpp"

#include <functional>
#include <memory>

namespace zim::database
{

    /**
     * A database client.
     *
     * @details A database client is a piece of code which interacts with a database.
     */
    class client
    {
    public:
        /**
         * Destructor.
         */
        virtual
        ~client() noexcept = default;

        /**
         * Set the session getter.
         *
         * @param session_getter The session getter.
         */
        void
        set_session_getter(session_getter_t session_getter)
        {
            m_session_getter = std::move(session_getter);
        }

    protected:
        // ToDo: Add timeout parameter
        std::shared_ptr<session>
        get_session() const
        {
            if (!m_session_getter)
                return { };

            return m_session_getter();
        }

    private:
        session_getter_t m_session_getter;
    };

}
