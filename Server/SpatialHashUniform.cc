#include <Server/SpatialHash.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

SpatialHash::SpatialHash(Simulation *sim) : simulation(sim), width(1), height(1) {}

void SpatialHash::refresh(uint32_t _width, uint32_t _height) {
    DEBUG_ONLY(assert(_width <= ARENA_WIDTH && _height <= ARENA_HEIGHT));
    width = div_round_up(_width, GRID_SIZE);
    height = div_round_up(_height, GRID_SIZE);
    for (uint32_t x = 0; x < MAX_GRID_X; ++x)
        for (uint32_t y = 0; y < MAX_GRID_Y; ++y)
            cells[x][y].clear();
}

void SpatialHash::insert(Entity const &ent) {
    DEBUG_ONLY(assert(ent.has_component(kPhysics));)
    //for the uniform grid to work, the max ent radius is GRID_SIZE/2
    //if larger entities are needed, either increase the GRID_SIZE
    //or use SpatialHashCanonical
    DEBUG_ONLY(assert(ent.get_radius() <= GRID_SIZE / 2);)
    uint32_t x = fclamp(ent.get_x(), 0, ARENA_WIDTH - 1) / GRID_SIZE;
    uint32_t y = fclamp(ent.get_y(), 0, ARENA_HEIGHT - 1) / GRID_SIZE;
    cells[x][y].push_back(ent.id);
}

void SpatialHash::collide(std::function<void(Simulation *, Entity &, Entity &)> on_collide) {
    for (uint32_t x = 0; x < MAX_GRID_X; ++x) {
        for (uint32_t y = 0; y < MAX_GRID_Y; ++y) {
            std::vector<EntityID> &cell = cells[x][y];
            for (uint32_t i = 0; i < cell.size(); ++i) {
                for (uint32_t j = i + 1; j < cell.size(); ++j) on_collide(simulation, simulation->get_ent(cell[i]), simulation->get_ent(cell[j]));
                if (x < MAX_GRID_X - 1) {
                    std::vector<EntityID> &cell2 = cells[x+1][y];
                    for (uint32_t j = 0; j < cell2.size(); ++j) on_collide(simulation, simulation->get_ent(cell[i]), simulation->get_ent(cell2[j]));
                    if (y > 0) {
                        std::vector<EntityID> &cell2 = cells[x+1][y-1];
                        for (uint32_t j = 0; j < cell2.size(); ++j) on_collide(simulation, simulation->get_ent(cell[i]), simulation->get_ent(cell2[j]));
                    }
                    if (y < MAX_GRID_Y - 1) {
                        std::vector<EntityID> &cell2 = cells[x+1][y+1];
                        for (uint32_t j = 0; j < cell2.size(); ++j) on_collide(simulation, simulation->get_ent(cell[i]), simulation->get_ent(cell2[j]));
                    }
                }
                if (y < MAX_GRID_Y - 1) {
                    std::vector<EntityID> &cell2 = cells[x][y+1];
                    for (uint32_t j = 0; j < cell2.size(); ++j) on_collide(simulation, simulation->get_ent(cell[i]), simulation->get_ent(cell2[j]));
                }
            }
        }
    }
}

void SpatialHash::query(float x, float y, float w, float h, std::function<void(Simulation *, Entity &)> cb) {
    uint32_t sx = fclamp(x - w - GRID_SIZE / 2, 0, ARENA_WIDTH - 1) / GRID_SIZE;
    uint32_t sy = fclamp(y - h - GRID_SIZE / 2, 0, ARENA_HEIGHT - 1) / GRID_SIZE;
    uint32_t ex = fclamp(x + w + GRID_SIZE / 2, 0, ARENA_WIDTH - 1) / GRID_SIZE;
    uint32_t ey = fclamp(y + h + GRID_SIZE / 2, 0, ARENA_HEIGHT - 1) / GRID_SIZE;
    for (uint32_t _x = sx; _x <= ex; ++_x) {
        for (uint32_t _y = sy; _y <= ey; ++_y) {
            std::vector<EntityID> &cell = cells[_x][_y];
            for (uint32_t i = 0; i < cell.size(); ++i) {
                Entity &ent = simulation->get_ent(cell[i]);
                if (ent.get_x() + ent.get_radius() < x - w) continue;
                if (ent.get_x() - ent.get_radius() > x + w) continue;
                if (ent.get_y() + ent.get_radius() < y - h) continue;
                if (ent.get_y() - ent.get_radius() > y + h) continue;
                cb(simulation, ent);
            }
        }
    }
}
