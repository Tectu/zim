#pragma once

namespace zim::database
{

    // ToDo: Add connection string
    struct config
    {
        /**
         * The size of the connection pool.
         */
        std::size_t connection_pool_size = 1;
    };

}
