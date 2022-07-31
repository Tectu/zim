#pragma once

#include <zim/app.hpp>

class database;
class page_master;

namespace apps::gallery
{
    namespace pages
    {
        class overview;
        class upload;
    }

    class app :
        public zim::app
    {
    public:
        app(
            std::shared_ptr<database> db,
            std::shared_ptr<page_master> master_page
        );

        ~app() noexcept override = default;

        [[nodiscard]]
        bool
        init() override;

    private:
        std::shared_ptr<database> m_db;
        std::shared_ptr<page_master> m_master_page;
        std::shared_ptr<pages::overview> m_page_overview;
        std::shared_ptr<pages::upload> m_page_upload;
    };
}
