#include "forms.hpp"

#include <malloy/core/html/form_renderer.hpp>

using namespace zim::pages;

forms::forms(
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
forms::data() const
{
    nlohmann::json data;

    data["forms"] = nlohmann::json::array();
    for (std::size_t i = 0; i < m_forms.size(); i++)
        data["forms"].emplace_back(m_form_renderer->render(*m_forms.at(i).form));

    return data;
}

bool
forms::add_form(form_record_t&& record)
{
    // Sanity checks
    if (record.name.empty())
        return false;
    if (!record.form)
        return false;
    if (!record.handler)
        return false;

    // Sanity check name
    {
        // Make sane requirements (prevent special characters etc, ...)
        // ToDo

        // Check if it exists already
        // ToDo
    }

    m_forms.emplace_back(std::move(record));

    return true;
}
