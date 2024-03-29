#include <zim/page/content.hpp>

#include "../../database/database.hpp"

namespace apps::gallery::pages
{

    class overview :
        public zim::pages::content
    {
    public:
        explicit
        overview(
            std::shared_ptr<zim::pages::master> master_page,
            std::shared_ptr<database> db
        ) :
            zim::pages::content(
                "../../../examples/demo_01/apps/gallery/assets/templates/overview.html",
                std::move(master_page)
            ),
            m_db{ std::move(db) }
        {
        }

    protected:
        [[nodiscard]]
        nlohmann::json
        data() const override
        {
            nlohmann::json j;

            j["images"] = nlohmann::json::array();
            for (const auto& img : m_db->images()) {
                nlohmann::json j_img;
                j_img["id"] = img.id;
                j_img["caption"] = img.caption;

                j["images"].emplace_back(std::move(j_img));
            }

            return j;
        }

    private:
        std::shared_ptr<database> m_db;
    };

}
