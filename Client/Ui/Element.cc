#include <Client/Ui/Element.hh>

using namespace Ui;

Element::Element(float w, float h) {
    width = w;
    height = h;
    animation = 1;
    visible = 1;
}

void Element::animate() {
    animation.lerp_step(visible, 0.2);
}

void Element::render(Renderer &ctx) {
    animate();
    on_render(ctx);
}