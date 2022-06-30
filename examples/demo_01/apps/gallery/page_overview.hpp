#include <zim/page_content.hpp>

namespace apps::gallery::pages
{

    class overview :
        public zim::page_content
    {
    public:
        explicit
        overview(
            std::shared_ptr<zim::page_master> master_page
        ) :
            zim::page_content(
                "../../../examples/demo_01/apps/gallery/assets/templates/overview.html",
                std::move(master_page)
            )
        {
        }

    protected:
        [[nodiscard]]
        nlohmann::json
        data() const override
        {
            return { };
        }
    };

}
