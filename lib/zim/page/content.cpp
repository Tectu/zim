#include "content.hpp"
#include "master.hpp"

using namespace zim::pages;

content::content(
    std::filesystem::path template_path,
    std::shared_ptr<master> master
):
    m_template_path(std::move(template_path)),
    m_master(std::move(master))
{
    // Sanity check
    if (!m_master)
        throw std::invalid_argument("no valid master page provided.");
}

malloy::http::response<>
content::render() const
{
    // Rendering is performed by the page::master class
    return m_master->render(*this);
}
