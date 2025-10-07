#include "DevState.hh"
#include <unordered_map>

static std::unordered_map<uint16_t, bool> IMMUNE;
static std::unordered_map<uint16_t, bool> SHOWTAG;

static bool get_or(const std::unordered_map<uint16_t,bool>& m, uint16_t k, bool defv) {
    auto it = m.find(k); return it==m.end()? defv : it->second;
}

bool dev_is_immune(uint16_t id)      { return get_or(IMMUNE,  id, true); }
void dev_set_immune(uint16_t id, bool v) { IMMUNE[id]  = v; }

bool dev_show_tag(uint16_t id)       { return get_or(SHOWTAG, id, true); }
void dev_set_show_tag(uint16_t id, bool v){ SHOWTAG[id] = v; }

void dev_toggle(uint16_t id) {
    IMMUNE[id]  = !dev_is_immune(id);
    SHOWTAG[id] = !dev_show_tag(id);
}