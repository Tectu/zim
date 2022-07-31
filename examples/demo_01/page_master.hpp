#include <zim/page/master.hpp>

class page_master :
    public zim::pages::master
{
public:
    page_master() :
        zim::pages::master("../../../examples/demo_01/assets/templates/master.html")
    {
    }

private:
    [[nodiscard]]
    nlohmann::json
    data() const override
    {
        nlohmann::json j;

        j["system"]["site_url"] = "http://127.0.0.1:8080/apps";
        j["system"]["assets_base_url"] = "http://127.0.0.1:8080/apps/app";

        return j;
    }
};
