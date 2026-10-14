#pragma once

#include <cstdint>
#include <string>

namespace DOM {
    void create_text_input(std::string_view, uint32_t);
    void element_show(std::string_view);
    void element_hide(std::string_view);
    void update_pos_and_dimension(std::string_view, float, float, float, float);
    std::string retrieve_text(std::string_view, uint32_t);
    void update_text(std::string_view, std::string_view, uint32_t);
    void open_page(std::string_view);
}