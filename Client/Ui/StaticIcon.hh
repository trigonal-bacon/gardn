#include <Client/Ui/Element.hh>

namespace Ui {
    class StaticIcon final : public Element {
        std::function<void(Element *, Renderer &)> icon_render_func;
    public:
        StaticIcon(std::function<void(Element *, Renderer &)>, float, float, Style = {});
        virtual void on_render(Renderer &) override;
    };
}