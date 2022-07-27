#pragma once

#include <soci/soci.h>

namespace zim::database
{

    /**
     * A database session.
     */
    using session = soci::session;
}
