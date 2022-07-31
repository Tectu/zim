#pragma once

#include "page.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <memory>

namespace zim::pages
{
    class master;

    /**
     * This type represents the content portion of a page.
     */
    class content :
        public page
    {
    public:
        content(
            std::filesystem::path template_path,
            std::shared_ptr<class master> master
        );

        content() = delete;
        ~content() noexcept override = default;

        [[nodiscard]]
        std::filesystem::path
        template_path() const noexcept
        {
            return m_template_path;
        }

        [[nodiscard]]
        virtual
        nlohmann::json
        data() const = 0;

        [[nodiscard]]
        malloy::http::response<>
        render() const override;

    private:
        std::filesystem::path m_template_path;
        std::shared_ptr<master> m_master;
    };

}
