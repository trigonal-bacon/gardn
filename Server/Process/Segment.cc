#include <Server/Process/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_segment_behavior(Simulation *sim, Entity &ent) {
    if (ent.is_tail && sim->ent_alive(ent.seg_head)) {
        Entity &par = sim->get_ent(ent.seg_head);
        Vector diff(ent.x - par.x, ent.y - par.y);
        diff.set_magnitude(ent.radius + par.radius + 0.01);
        ent.set_x(par.x + diff.x);
        ent.set_y(par.y + diff.y);
        ent.set_angle(diff.angle() + M_PI);
        if (sim->ent_alive(par.target))
            ent.target = par.target;
    }
}