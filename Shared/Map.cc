#include <Shared/Map.hh>

#ifdef SERVERSIDE
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#endif

#include <cmath>
#include <iostream>

using namespace Map;

uint32_t Map::difficulty_at_level(uint32_t level) {
  if (level / LEVELS_PER_EXTRA_SLOT > MAX_DIFFICULTY)
    return MAX_DIFFICULTY;
  return level / LEVELS_PER_EXTRA_SLOT;
}

uint32_t Map::get_zone_from_pos(float x, float y) {
  uint32_t ret = 0;
  for (uint32_t i = 1; i < MAP_DATA.size(); ++i) {
    const ZoneDefinition& zone = MAP_DATA[i];
    if (fclamp(x, zone.left, zone.right) == x &&
        fclamp(y, zone.top, zone.bottom) == y)
      ret = i;
  }
  return ret;
}

uint32_t Map::get_suitable_difficulty_zone(uint32_t power) {
  std::vector<uint32_t> possible_zones;
  for (uint32_t i = 0; i < MAP_DATA.size(); ++i)
    if (MAP_DATA[i].difficulty == power)
      possible_zones.push_back(i);
  if (possible_zones.size() == 0)
    return 0;
  return possible_zones[frand() * possible_zones.size()];
}
