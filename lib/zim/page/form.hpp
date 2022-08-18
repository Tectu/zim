#pragma once

#include "content.hpp"

#include <malloy/core/http/request.hpp>

#include <functional>

namespace malloy::html
{
    class form;
    class form_data;
    struct form_renderer;
}

namespace zim
{
    class app;
}

namespace zim::pages
{
    class master;

    /**
     * A type to represent a page which only consists of an HTML form.
     */
    class form :
        public content
    {
        friend class zim::app;

    public:
        /**
         * Handler for POST requests.
         */
        using handler_t = std::function<malloy::http::response<>(const malloy::http::request<>&, const malloy::html::form_data&)>;

        form(
            std::filesystem::path template_path,
            std::shared_ptr<class master> master
        );

        form() = delete;
        ~form() noexcept override = default;

        [[nodiscard]]
        nlohmann::json
        data() const override;

    protected:
        std::shared_ptr<malloy::html::form> m_form;
        std::shared_ptr<const malloy::html::form_renderer> m_form_renderer;
        handler_t m_handler;
    };

}
