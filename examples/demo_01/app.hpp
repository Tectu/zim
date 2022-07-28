#pragma once

#include <zim/app.hpp>

class database;
class page_master;

/**
 * Top-level application.
 */
class app :
    public zim::app
{
public:
    explicit app(std::shared_ptr<database> db);

    ~app() override = default;

    [[nodiscard]]
    bool init() override;

private:
    std::shared_ptr<database> m_db;
    std::shared_ptr<page_master> m_master_page;
};
