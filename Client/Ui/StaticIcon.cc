#include <Client/Ui/StaticIcon.hh>

#include <Helpers/Macros.hh>

using namespace Ui;

StaticIcon::StaticIcon(std::function<void(Element *, Renderer &)> render_func, float w, float h, Style s)
    : Element(w, h, s), icon_render_func(std::move(render_func)) {
    assert(render_func != nullptr);
}

void StaticIcon::on_render(Renderer &ctx) {
    icon_render_func(this, ctx);
}