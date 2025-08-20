#include <Server/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_chat_behavior(Simulation *sim, Entity &ent) {
    if (!sim->ent_alive(ent.parent)) return;
    Entity &parent = sim->get_ent(ent.parent);
    if (parent.chat_sent != NULL_ENTITY && parent.chat_sent != ent.id) {
        ++ent.chat_pos;
        if (ent.chat_pos >= CHAT_SIZE) sim->request_delete(ent.id);
    }
    ent.set_x(parent.x);
    ent.set_y(parent.y - parent.radius - 45 - 28 * ent.chat_pos);
}
