#pragma once

#include <soci/soci.h>

#include <string>

/**
 * An image.
 */
struct image
{
    int id;                 ///< Database ID.
    std::string caption;    ///< Caption.
    std::string data;       ///< The binary data (the actual image).
};

namespace soci
{
    template<>
    struct type_conversion<image>
    {
        typedef values base_type;

        static
        void
        from_base(const values& v, [[maybe_unused]] indicator ind, image& img)
        {
            img.id = v.get<int>("id");
            img.caption = v.get<std::string>("caption");
            img.data = v.get<std::string>("data");
        }

        static
        void
        to_base(const image& img, values& v, indicator& ind)
        {
            v.set("id", img.id);
            v.set("caption", img.caption);
            v.set("data", img.data);

            ind = i_ok;
        }
    };
}