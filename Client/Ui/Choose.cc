#include <Client/Ui/Choose.hh>

#include <Client/Ui/Extern.hh>

#include <Helpers/Macros.hh>

using namespace Ui;

uint8_t no_show() { return 0; }
uint8_t do_show() { return 1; }

Choose::Choose(std::unique_ptr<Element> l, std::unique_ptr<Element> r,
               const std::function<uint8_t(void)>& c, Style s)
    : Container(make_children(std::move(l), std::move(r)), 0, 0, s),
      chooser(std::move(c)), choose_showing(c()) {
  children[0]->showed = 1;
  children[1]->showed = 1;
  children[0]->refactor();
  children[1]->refactor();
  children[1]->animation.set(0);
  refactor();
}

void Choose::on_render(Renderer& ctx) {
  DEBUG_ONLY(assert(choose_showing < children.size());)
  auto& rendering = children[choose_showing];
  rendering->render(ctx);
  if (!rendering->visible)
    children[1 - choose_showing]->render(ctx);
}

void Choose::refactor() {
  uint8_t to_show = chooser();
  children[to_show]->style.should_render = do_show;
  children[1 - to_show]->style.should_render = no_show;
  if (!children[choose_showing]->visible || !showed)
    choose_showing = to_show;
  auto& rendering = children[choose_showing];
  rendering->refactor();
  width = rendering->width;
  height = rendering->height;
}

void Choose::poll_events(const ScreenEvent& event) {
  if (style.no_polling)
    return;
  Element::poll_events(event);
  if (Ui::focused != this)
    return;
  for (auto& child : children)
    if (child->visible)
      child->poll_events(event);
  // if (children[0]->visible) children[0]->poll_events(event);
  // if (children[1]->visible) children[1]->poll_events(event);
}