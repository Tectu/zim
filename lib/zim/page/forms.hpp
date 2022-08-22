#pragma once

#include "content.hpp"

#include <malloy/core/http/request.hpp>

#include <functional>
#include <tuple>

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
     * A type to represent a page which only consists of multiple HTML forms.
     *
     * @details Forms are identified by a string.
     */
    class forms :
        public content
    {
        friend class zim::app;

    public:
        /**
         * Handler for POST requests.
         */
        using handler_t = std::function<malloy::http::response<>(const malloy::http::request<>&, const malloy::html::form_data&)>;

        struct form_record_t {
            std::string name;
            std::shared_ptr<malloy::html::form> form;
            handler_t handler;
        };

        forms(
            std::filesystem::path template_path,
            std::shared_ptr<class master> master
        );

        forms() = delete;
        ~forms() noexcept override = default;

        [[nodiscard]]
        nlohmann::json
        data() const override;

    protected:
        std::shared_ptr<const malloy::html::form_renderer> m_form_renderer;

        bool
        add_form(form_record_t&& record);

    private:
        std::vector<form_record_t> m_forms;
    };

}
