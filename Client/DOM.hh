#pragma once

#include <cstdint>
#include <string>

namespace DOM {
    void create_text_input(char const *, uint32_t);
    void element_show(char const *);
    void element_hide(char const *);
    void update_pos_and_dimension(char const *, float, float, float, float);
    std::string retrieve_text(char const *, uint32_t);
    void update_text(char const *, std::string const &, uint32_t);
    void element_focus(char const *);
    void blur();
    void open_page(char const *);
    void reload_page();
    void toggle_fullscreen();
}