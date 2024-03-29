#pragma once

#include "page.hpp"

#include <malloy/core/http/response.hpp>
#include <nlohmann/json.hpp>

#include <filesystem>

namespace spdlog
{
    class logger;
}

namespace zim::pages
{
    class content;

    /**
     * Class for rendering template based HTML pages.
     *
     * @note This uses the pantor/inja template engine. See the corresponding docs for capabilities.
     */
    class master :
        public page
    {
    public:
        explicit
        master(
            std::filesystem::path template_path,
            std::shared_ptr<spdlog::logger> logger = { }
        );

        master() = delete;
        ~master() override = default;

        /**
         * Renders the template without any content.
         *
         * @return The HTTP response.
         */
        [[nodiscard]]
        malloy::http::response<>
        render() const override;

        /**
         * Renders a template with content.
         *
         * @param content The content.
         * @return The HTTP response.
         */
        [[nodiscard]]
        malloy::http::response<>
        render(const class content& content) const;

    protected:
        /**
         * Renders a template with optional content.
         *
         * @param data The template data.
         * @param content The content page (if any).
         * @return The HTTP response.
         */
        [[nodiscard]]
        malloy::http::response<>
        render(const nlohmann::json& data, const class content* content) const;

    private:
        std::shared_ptr<spdlog::logger> m_logger;
        std::filesystem::path m_tmpl_path;

        [[nodiscard]]
        virtual
        nlohmann::json
        data() const = 0;
    };

}
