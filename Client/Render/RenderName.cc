#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Game.hh>

#include <Shared/Entity.hh>

void render_name(Renderer &ctx, Entity const &ent) {
    if (!ent.get_nametag_visible()) return;
    if (ent.id == Game::player_id) return;
    ctx.translate(0, -ent.get_radius() - 18);
    ctx.set_global_alpha(1 - ent.deletion_animation);
    ctx.scale(1 + 0.5 * ent.deletion_animation);
    ctx.draw_text(ent.get_name().c_str(), { .size = 18 });
}