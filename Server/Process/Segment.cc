#include <Server/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_segment_behavior(Simulation *sim, Entity &ent) {
    if (sim->ent_alive(ent.seg_head)) {
        Entity &par = sim->get_ent(ent.seg_head);
        Vector diff(ent.get_x() - par.get_x(), ent.get_y() - par.get_y());
        diff.set_magnitude(ent.get_radius() + par.get_radius() + 0.01);
        ent.set_x(par.get_x() + diff.x);
        ent.set_y(par.get_y() + diff.y);
        ent.set_angle(diff.angle() + M_PI);
        if (sim->ent_alive(par.target))
            ent.target = par.target;
    }
}