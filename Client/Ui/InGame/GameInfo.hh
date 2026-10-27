#pragma once

#include <Client/Ui/Element.hh>
#include <Client/Ui/Container.hh>

namespace Ui {
class LevelBar final : public Element {
public:
  LerpFloat progress;
  uint32_t level;
  LevelBar();
  virtual void on_render(Renderer&) override;
};

class LeaderboardSlot final : public Element {
public:
  uint8_t pos;
  LerpFloat ratio;
  LeaderboardSlot(uint8_t);

  virtual void on_render(Renderer&) override;
};

class Minimap final : public Element {
public:
  Minimap(float);
  virtual void on_render(Renderer&) override;
};

class OverlevelTimer final : public Element {
public:
  OverlevelTimer(float);
  virtual void on_render(Renderer&) override;
};

class MobileJoyStick final : public Element {
  float joystick_x;
  float joystick_y;
  float joystick_radius;
  uint32_t persistent_touch_id;
  uint8_t is_pressed;

public:
  MobileJoyStick(float, float, float);
  virtual void on_render(Renderer&) override;
  virtual void on_event(uint8_t) override;
};

class Notification final : public HContainer {
  float creation_time;
  float duration;
  uint8_t should_hide;
public:
  Notification(std::string_view, float);
};

class NotificationContainer final : public VContainer {
public:
  NotificationContainer();
  virtual void on_render(Renderer&) override;
  virtual void refactor() override;
};

std::unique_ptr<Element> make_leaderboard();
std::unique_ptr<Element> make_level_bar();
std::unique_ptr<Element> make_minimap();
std::unique_ptr<Element> make_overlevel_indicator();
std::unique_ptr<Element> make_mobile_attack_button();
std::unique_ptr<Element> make_mobile_defend_button();
std::unique_ptr<Element> make_mobile_joystick();
std::unique_ptr<Element> make_notification_container();
} // namespace Ui