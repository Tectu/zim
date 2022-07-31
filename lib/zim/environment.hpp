#pragma once

#include <filesystem>
#include <string>

namespace zim
{

    /**
     * Environment. Mainly used by @p app.
     */
    struct environment
    {
        /**
         * Site related stuff.
         */
        struct {
            std::string base_url;
        } site;

        /**
         * App related stuff.
         */
        struct {
            std::string base_url;
        } app;

        /**
         * Makes a sub-environment.
         *
         * This is mainly used by apps to create an environment for a sub app.
         *
         * @param name The name of the sub-app.
         * @return The sub-environment.
         */
        [[nodiscard]]
        environment
        make_sub_environment(const std::string& name)
        {
            environment env{ *this };
            env.app.base_url += "/" + name;

            return env;
        }
    };

}
