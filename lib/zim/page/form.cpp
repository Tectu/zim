#include "form.hpp"

#include <malloy/core/html/form_renderer.hpp>

using namespace zim::pages;

form::form(
    std::filesystem::path template_path,
    std::shared_ptr<class master> master
) :
    content(
        std::move(template_path),
        std::move(master)
    )
{
}

nlohmann::json
form::data() const
{
    nlohmann::json data;

    data["form"] = m_form_renderer->render(*m_form);

    return data;
}
