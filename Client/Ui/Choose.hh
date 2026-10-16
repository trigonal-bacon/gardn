#pragma once

#include <Client/Ui/Container.hh>

namespace Ui {
class Choose : public Container {
  const std::function<uint8_t(void)> chooser;
  uint8_t choose_showing;

public:
  Choose(std::unique_ptr<Element>, std::unique_ptr<Element>,
         const std::function<uint8_t(void)>&, Style = {});

  virtual void on_render(Renderer&) override;
  virtual void refactor() override;
  virtual void poll_events(const ScreenEvent&) override;
};
} // namespace Ui