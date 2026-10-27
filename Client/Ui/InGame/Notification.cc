#include <Client/Ui/InGame/GameInfo.hh>

#include <Client/Game.hh>
#include <Client/Debug.hh>
#include <Client/Ui/StaticText.hh>

#include <algorithm>

using namespace Ui;

Notification::Notification(std::string_view message, float duration) 
  : HContainer(make_children(
        el<StaticText>(16, message)
    ), 5, 0, Style{.fill = 0x40000000, .round_radius = 5, .v_justify = Style::Top}),
    creation_time(Game::timestamp), should_hide(0), duration(duration) {
    style.should_render = [this](){
        bool showing = Game::timestamp < this->creation_time + this->duration && !should_hide;
        should_hide = !showing;
        return showing;
    };
    showed = 1; //animate both ways
    animation.set(0);
}

NotificationContainer::NotificationContainer() : VContainer(make_children(), 0, 10, {}) {}

void NotificationContainer::on_render(Renderer& ctx) {
    Container::on_render(ctx);
    std::erase_if(children, [](const auto& elt){
        return bool(!elt->visible);
    });
    //add new notifs here
    for (const auto& notif : Game::notifications) {
        add_child(el<Notification>(notif.message, notif.duration));
    }
}

void NotificationContainer::refactor() {
  float x = 0;
  float y = outer_pad;
  for (auto& elt : children) {
    // elt->style.v_justify = -1;
    elt->refactor();
    if (!elt->visible)
      continue;
    // if (!elt->rendering || elt->detached) continue;
    // Layout l = elt->get_layout();
    elt->x = -elt->style.h_justify * outer_pad;
    elt->y = y;
    x = std::max<float>(x, elt->width * elt->animation);
    y += elt->height * elt->animation + inner_pad;
  }
  x += 2 * outer_pad;
  y += outer_pad - inner_pad;
  width = x;
  height = y;
}

std::unique_ptr<Element> Ui::make_notification_container() {
    std::unique_ptr<Element> elt = static_cast<std::unique_ptr<Element>>(el<NotificationContainer>());
    elt->x = 0;
    elt->y = 50;
    elt->style.v_justify = Style::Top;
    elt->style.h_justify = Style::Middle;
    return elt;
}