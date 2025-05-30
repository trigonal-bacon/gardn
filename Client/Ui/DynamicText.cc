#include <Client/Ui/DynamicText.hh>

#include <Client/Game.hh>

using namespace Ui;

DynamicText::DynamicText(float size, std::function<std::string(void)> genfunc, Style s)
 : Element(0, size, s), generator(genfunc)
{
}

void DynamicText::refactor() {
    text = generator();
    Game::renderer.set_text_size(height);
    width = Game::renderer.get_text_size(text.c_str());
}

void DynamicText::on_render(Renderer &ctx) {
    ctx.draw_text(text.c_str(), { .fill = style.fill, .size = height });
}