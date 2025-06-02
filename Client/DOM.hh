#pragma once

#include <cstdint>
#include <string>

namespace DOM {
    void create_text_input(char const *, uint32_t);
    void element_show(char const *);
    void element_hide(char const *);
    void update_pos_and_dimension(char const *, float, float, float, float);
    std::string retrieve_text(char const *, uint32_t);
}