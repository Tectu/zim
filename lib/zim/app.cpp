#include "app.hpp"
#include "page/form.hpp"
#include "page/forms.hpp"
#include "page/page.hpp"

#include <malloy/core/html/form.hpp>

#include <stdexcept>

using namespace zim;

bool
app::add_page(std::string&& target, std::shared_ptr<pages::page> page)
{
    // Sanity check
    if (!page || !m_router)
        return false;

    // Add endpoint
    return m_router->add(
        malloy::http::method::get,
        std::move(target),
        [page = std::move(page)]([[maybe_unused]] const auto& req) {
            return page->render();
        }
    );
}

bool
app::add_form(std::string&& target, std::shared_ptr<pages::form> page)
{
    // Sanity check
    if (!page || !m_router)
        return false;

    // Add GET endpoint
    if (
        !m_router->add(
            malloy::http::method::get,
            target,
            [page]([[maybe_unused]] const auto& req) {
                return page->render();
            }
        )
    )
        return false;

    // Add POST endpoint
    if (
        !m_router->add(
            malloy::http::method::post,
            std::move(target),
            [page](const auto& req) {
                // Sanity check
                if (!page->m_form)
                    return malloy::http::generator::server_error("page holds no form.");

                // Sanity check
                if (!page->m_handler)
                    return malloy::http::generator::server_error("page holds no handler.");

                // Parse form
                const auto& data = page->m_form->parse(req);
                if (!data)
                    return malloy::http::generator::bad_request("invalid form data.");

                // Invoke handler
                return page->m_handler(req, *data);
            }
        )
    )
        return false;

    return true;
}

bool
app::add_form(std::string&& target, std::shared_ptr<pages::forms> page)
{
    // Sanity check
    if (!page || !m_router)
        return false;

    // Add GET endpoint
    if (
        !m_router->add(
            malloy::http::method::get,
            target,
            [page]([[maybe_unused]] const auto& req) {
                return page->render();
            }
        )
    )
        return false;

    // Add POST endpoints
    for (const auto& form_record : page->m_forms) {

        if (
            !m_router->add(
                malloy::http::method::post,
                target + "/" + form_record.name,
                [form_record](const auto& req) {
                    // Sanity check
                    if (!form_record.form)
                        return malloy::http::generator::server_error("page holds no form with name \"" + form_record.name + "\".");

                    // Sanity check
                    if (!form_record.handler)
                        return malloy::http::generator::server_error("page holds no handler for form with name \"" + form_record.name + "\".");

                    // Parse form
                    const auto& data = form_record.form->parse(req);
                    if (!data)
                        return malloy::http::generator::bad_request("invalid form data.");

                    // Invoke handler
                    return form_record.handler(req, *data);
                }
            )
        )
            return false;
    }

    return true;
}
