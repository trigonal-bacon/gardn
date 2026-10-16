#pragma once

#include <Client/Render/Renderer.hh>

#include <Helpers/Math.hh>

#include <functional>
#include <memory>
#include <vector>

namespace Ui {

struct ScreenEvent {
  uint32_t id = (uint32_t)-1;
  float x = 0;
  float y = 0;
  uint8_t press = 0;
};

class Element;

struct Style {
  enum { Top = -1, Middle = 0, Bottom = 1, Left = -1, Center = 0, Right = 1 };
  uint32_t fill = 0x00000000;
  float stroke_hsv = 0.8;
  float line_width = 3;
  float round_radius = 0;
  std::function<void(Element&, Renderer&)> animate = nullptr;
  std::function<bool(void)> should_render = nullptr;
  int8_t h_justify = Center;
  int8_t v_justify = Middle;
  uint8_t layer = 0;
  uint8_t no_animation = 0;
  uint8_t no_polling = 0;
};

enum UiEvent {
  kFocusLost,
  kMouseDown,
  kClick,
  kMouseUp,
  kMouseMove,
  kMouseHover
};

namespace Registry {
Element* find(uint32_t);
}

class Element {
public:
  template <typename T>
    requires std::derived_from<T, Element>
  class Handle {
  private:
    uint32_t handle = 0;

  public:
    Handle(T* elt = nullptr) {
      if (elt != nullptr)
        handle = elt->handle;
      else
        handle = 0;
    };
    bool operator==(const Handle<T>& other) const {
      return handle == other.handle;
    };
    T* get() const { return static_cast<T*>(Registry::find(handle)); }
    operator bool() const { return get() != nullptr; }
  };

private:
  uint32_t handle;

protected:
  std::vector<std::unique_ptr<Element>> children;
  Handle<Element> tooltip;
  uint32_t touch_id = (uint32_t)-1;

public:
  Handle<Element> parent;
  float width = 0;
  float height = 0;
  float x = 0;
  float y = 0;
  float screen_x = 0;
  float screen_y = 0;
  LerpFloat animation;
  LerpFloat tooltip_animation;
  Ui::Style style;

  uint8_t focus_state : 3 = 0;
  uint8_t visible : 1 = 1;
  uint8_t showed : 1 = 0;
  uint8_t rendering_tooltip : 1 = 0;
  uint8_t focused : 1 = 0;

  Element(float = 0, float = 0, Style = {});
  Element(const Element&) = delete;
  Element& operator=(const Element&) = delete;
  Element(Element&&) = delete;
  Element& operator=(Element&&) = delete;
  virtual ~Element();
  void add_child(std::unique_ptr<Element>);
  void render(Renderer&);
  virtual void on_render(Renderer&);
  virtual void on_render_tooltip(Renderer&);
  virtual void on_render_skip(Renderer&);
  virtual void on_event(uint8_t);
  virtual void refactor();
  virtual void poll_events(const ScreenEvent&);
};

std::vector<std::unique_ptr<Element>>
make_range(uint32_t, uint32_t, std::unique_ptr<Element>(uint32_t));

template <typename T, typename... Args> auto el(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename... Ptrs> auto make_children(Ptrs&&... ptrs) {
  std::vector<std::unique_ptr<Element>> result;
  result.reserve(sizeof...(Ptrs));
  (result.push_back(std::forward<Ptrs>(ptrs)), ...);
  return result;
}
} // namespace Ui