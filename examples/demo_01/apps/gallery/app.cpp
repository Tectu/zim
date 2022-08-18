#include "app.hpp"
#include "page_overview.hpp"
#include "page_upload.hpp"
#include "../../database/database.hpp"
#include "../../page_master.hpp"

#include <malloy/server/routing/router.hpp>

using namespace apps::gallery;

app::app(
    std::shared_ptr<database> db,
    std::shared_ptr<page_master> master_page
) :
    m_db(std::move(db)),
    m_master_page(std::move(master_page))
{
    // Sanity check database
    if (!m_db)
        throw std::invalid_argument("no valid database provided.");
}

bool
app::init()
{
    using namespace malloy::http;

    // Setup pages
    {
        m_page_overview = std::make_shared<pages::overview>(m_master_page, m_db);
        m_page_upload = std::make_shared<pages::upload>(m_master_page);
    }

    // Setup router
    {
        // Root page
        add_page("", m_page_overview);

        // Upload page
        add_page("/upload", m_page_upload);

        // Upload POST endpoint
        m_router->add(method::post, "/upload", [this](const auto& req){
            auto form = *m_page_upload->m_form;

            // Parse form
            const auto& data = form.parse(req);
            if (!data)
                return generator::bad_request("invalid form data.");

            // Re-populate the form's pre-filled values
            form.populate_values_from_parsed_data(*data);

            // Extract values & perform sanity checks
            image img;
            img.caption = data->content("caption").value_or("");
            img.data = data->content("image").value_or("");
            {
                if (img.caption.empty())
                    return generator::bad_request("caption must not be empty.");

                if (img.data.empty())
                    return generator::bad_request("image must not be empty.");
            }

            // Insert into database
            if (const bool successful = m_db->add_image(img); !successful) {
                m_logger->warn("could not insert image into database.");

                return generator::server_error("could not store image in database.");
            }

            // Upload was successful, redirect to root page
            return generator::redirect(status::see_other, "");
        });

        // GET image endpoint
        m_router->add(method::get, "/image/(\\d+)", [this](const auto& req, const auto& captures) {
            if (captures.size() != 1)
                return generator::bad_request("invalid image id");

            try {
                const int id = std::stoi(captures[0]);
                const auto& image_opt = m_db->get_image(id);
                if (!image_opt)
                    return generator::bad_request("could not retrieve image by id.");

                // Assemble response
                //response<boost::beast::http::file_body> resp{status::ok};
                response resp{status::ok};
                //resp.set(field::content_type, mime_type);
                resp.body() = image_opt->data;
                resp.prepare_payload(); // ToDo: Is this needed or does malloy handle that for us?

                return resp;
            }
            catch (const std::exception& e) {
                m_logger->warn("could not retrieve image from database. exception: {}", e.what());
                return generator::server_error("could not get image from database.");
            }
        });
    }

    return true;
}
