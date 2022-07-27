#include "app.hpp"
#include "page.hpp"

#include <stdexcept>

using namespace zim;

app::app(
    std::shared_ptr<spdlog::logger> logger,
    std::string name,
    environment env
) :
    m_env(std::move(env)),
    m_logger(std::move(logger)),
    m_name(std::move(name))
{
    // Sanity check logger
    if (!m_logger)
        throw std::invalid_argument("no valid logger provided.");

    // Sanity check name
    if (m_name.empty())
        throw std::invalid_argument("application name must not be empty.");

    m_router = std::make_unique<malloy::server::router>(m_logger->clone("foobar"));
}

bool
app::add_page(std::string&& target, std::shared_ptr<page> page)
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
