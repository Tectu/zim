#pragma once

#include "session.hpp"

#include <functional>
#include <memory>

namespace zim::database
{

    /**
     * Getter to retrieve a session from the database manager.
     */
    using session_getter_t = std::function<std::shared_ptr<database::session>()>;

}
