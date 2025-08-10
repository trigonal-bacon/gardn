#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Game.hh>

#include <Shared/Entity.hh>

void render_name(Renderer &ctx, Entity const &ent) {
    ctx.translate(0, -ent.radius - 18);
    ctx.set_global_alpha(1 - ent.deletion_animation);
    ctx.scale(1 + 0.5 * ent.deletion_animation);

    bool has_name = ent.nametag_visible && !(ent.id == Game::player_id) && ent.name.size() > 0;
    if (has_name)
        ctx.draw_text(ent.name.c_str(), { .size = 18 });

    if (!ent.chat_message.empty() && ent.chat_timer > 0) {
        RenderContext c(&ctx);
        int bubble_up = has_name ? -46 : -28;
        ctx.translate(0, bubble_up);
        float alpha = ent.chat_timer > 1.0f ? 1.0f : fclamp(ent.chat_timer, 0.0f, 1.0f);
        ctx.set_global_alpha(alpha);

        ctx.set_text_size(14);
        float text_width = ctx.get_text_size(ent.chat_message.c_str());
        float bubble_width = text_width + 12; 
        float bubble_height = 16 + 6; 

        ctx.set_fill(0xFFFFE000); 
        ctx.set_stroke(0x000000FF); 
        ctx.set_line_width(2);

        ctx.begin_path();
        ctx.round_rect(-bubble_width/2, -bubble_height/2, bubble_width, bubble_height, 6); // more rounded like screenshot
        ctx.fill();
        ctx.stroke();

        ctx.draw_text(ent.chat_message.c_str(), { .fill = 0xFFFFFFFF, .size = 14 });
    }
}