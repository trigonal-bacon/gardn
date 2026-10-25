#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Game.hh>

#include <Shared/Entity.hh>

#include <cmath>

void render_animation(Renderer& ctx, const Entity& ent) {
   if (Game::simulation.ent_exists(ent.get_parent())) return;
   ctx.set_global_alpha(0.5 * (1 - ent.deletion_animation));
   ctx.set_stroke(0xffffffff);
   ctx.set_line_width(ent.get_radius());
   ctx.begin_path();
   ctx.move_to(ent.get_x(), ent.get_y());
   std::reference_wrapper<const Entity> last = ent;
   while (!last.get().children.empty()) {
      const Entity& child = Game::simulation.get_ent(last.get().children[0]);
      Vector last_pos = Vector{last.get().get_x(), last.get().get_y()};
      Vector curr_pos = Vector{child.get_x(), child.get_y()};
      SeedGenerator gen(ent.id.id * 374572 + 46237);
      while (1) {
         Vector delta = curr_pos - last_pos;
         float magnitude = delta.magnitude();
         float disp = gen.binext() * 5 + 30;
         if (disp >= magnitude) break;
         float angle = delta.angle() + gen.binext() * M_PI / 6;
         Vector new_pos = last_pos + Vector().unit_normal(angle) * disp;
         ctx.line_to(new_pos.x, new_pos.y);
         last_pos = new_pos;
      }
      ctx.line_to(child.get_x(), child.get_y());
      last = child;
   }
   ctx.stroke();
   ctx.set_stroke(0xffccccff);
   ctx.set_line_width(ent.get_radius() * 0.5);
   ctx.stroke();
}