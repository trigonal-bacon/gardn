#include <Server/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_chat_behavior(Simulation *sim, Entity &ent) {
    if (!sim->ent_alive(ent.get_parent())) return;
    Entity &parent = sim->get_ent(ent.get_parent());
    if (parent.chat_sent != NULL_ENTITY && parent.chat_sent != ent.id) {
        ++ent.chat_pos;
        if (ent.chat_pos >= CHAT_SIZE) sim->request_delete(ent.id);
    }
    ent.set_x(parent.get_x());
    ent.set_y(parent.get_y() - parent.get_radius() - 45 - 28 * ent.chat_pos);
}
