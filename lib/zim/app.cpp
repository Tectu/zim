#include "app.hpp"
#include "page/page.hpp"

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
