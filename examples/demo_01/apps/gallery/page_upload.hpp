#include "../../form_renderer.hpp"
#include "../../database/database.hpp"

#include <malloy/core/html/form.hpp>
#include <malloy/core/html/form_renderer.hpp>
#include <spdlog/logger.h>
#include <zim/page/form.hpp>

namespace apps::gallery::pages
{

    class upload:
        public zim::pages::form
    {
    public:
        explicit
        upload(
            std::shared_ptr<zim::pages::master> master_page,
            std::shared_ptr<spdlog::logger> logger,
            std::shared_ptr<database> db
        ) :
            zim::pages::form(
                "../../../examples/demo_01/apps/gallery/assets/templates/upload.html",
                std::move(master_page)
            ),
            m_logger{ std::move(logger) },
            m_db{ std::move(db) }
        {
            // Setup form
            {
                // Create form
                m_form = std::make_shared<malloy::html::form>(
                    malloy::http::method::post,
                    "http://127.0.0.1:8080/apps/app/gallery/upload",
                    malloy::html::form::encoding::multipart
                );

                // Caption field
                m_form->add_field({
                    .name = "caption",
                    .type = "text",
                    .label = "Caption:",
                    .required = true,
                });

                // Image field
                m_form->add_field({
                    .name = "image",
                    .type = "file",
                    .label = "Image:",
                    .required = true,
                });

                // Submit button
                m_form->add_field({
                    .name = "Upload!",
                    .type = "submit"
                });
            }

            // Setup form renderer
            m_form_renderer = std::make_shared<form_renderer>();

            // Setup form handler
            {
                m_handler = [this](const auto& req, const auto& form_data) {
                    using namespace malloy::http;

                    // Re-populate the form's pre-filled values
                    // ToDo: This needs to be handled/implemented properly. Ideally supported by the underlying class.
                    //form.populate_values_from_parsed_data(*data);

                    // Extract values & perform sanity checks
                    image img;
                    img.caption = form_data.content("caption").value_or("");
                    img.data = form_data.content("image").value_or("");
                    {
                        // Note: These checks are not really necessary as malloy's form parser ensures that all fields
                        //       marked as `required` are present in the supplied form data.

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
                };
            }
        }

    private:
        std::shared_ptr<spdlog::logger> m_logger;
        std::shared_ptr<database> m_db;
    };

}
