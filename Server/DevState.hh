#pragma once
#include <cstdint>

bool dev_is_immune(uint16_t id);         // default true for devs
void dev_set_immune(uint16_t id, bool v);

bool dev_show_tag(uint16_t id);          // default true for devs
void dev_set_show_tag(uint16_t id, bool v);

void dev_toggle(uint16_t id);            // flips both