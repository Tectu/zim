#pragma once

#include <string>

namespace zim::database
{

    // ToDo: Add connection string
    struct config
    {
        /**
         * The database type string.
         */
        std::string type;

        /**
         * The connection string.
         */
        std::string connection_string;

        /**
         * The size of the connection pool.
         */
        std::size_t connection_pool_size = 1;
    };

}
