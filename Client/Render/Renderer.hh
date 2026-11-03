#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

class Renderer;

class RenderContext {
public:
  std::array<float, 6> transform_matrix;
  uint32_t color_filter = 0;
  float amount = 0;
  float clip_x = 0;
  float clip_y = 0;
  float clip_w = 0;
  float clip_h = 0;
  void reset(const Renderer&);
};

class RenderGuard : private RenderContext {
protected:
  friend class Renderer;
  Renderer& renderer;
public:
  RenderGuard(Renderer&);
  RenderGuard(const RenderGuard&) = delete;
  RenderGuard& operator=(const RenderGuard&) = delete;
  RenderGuard(RenderGuard&&) = delete;
  RenderGuard& operator=(RenderGuard&&) = delete;
  ~RenderGuard();
};

class Renderer {
public:
  static std::vector<Renderer*> renderers;
  struct TextArgs {
    uint32_t fill = 0xffffffff;
    uint32_t stroke = 0xff000000;
    float size = 0;
    float stroke_scale = 0.12;
  };

  RenderContext context;
  uint32_t id = 0;
  float width = 0;
  float height = 0;
  Renderer();
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;
  ~Renderer();

  static uint32_t HSV(uint32_t, float);
  static uint32_t MIX(uint32_t, uint32_t, float);
  static float get_ascii_text_size(std::string_view);

  void reset();

  void set_dimensions(float, float);

  void set_color_filter(uint32_t, float);
  void set_fill(uint32_t);
  void set_stroke(uint32_t);

  void round_line_join();
  void round_line_cap();
  void center_text_align();
  void center_text_baseline();
  void set_line_width(float);
  void set_text_size(float);
  void set_global_alpha(float);

  void set_transform(float, float, float, float, float, float);
  void scale(float);
  void scale(float, float);
  void translate(float, float);
  void rotate(float);
  void reset_transform();
  void begin_path();
  void move_to(float, float);
  void line_to(float, float);
  void qcurve_to(float, float, float, float);
  void bcurve_to(float, float, float, float, float, float);

  void partial_arc(float, float, float, float, float, uint8_t);
  void arc(float, float, float);
  void reverse_arc(float, float, float);
  void ellipse(float, float, float, float);
  void ellipse(float, float, float, float, float);
  void rect(float, float, float, float);
  void round_rect(float, float, float, float, float);
  void fill_rect(float, float, float, float);
  void stroke_rect(float, float, float, float);

  void close_path();
  void fill(uint8_t = 0);
  void stroke();
  void clip();
  void clip_rect(float, float, float, float);

  void draw_image(Renderer&);

  void fill_text(std::string_view);
  void stroke_text(std::string_view);
  void draw_text(std::string_view, const TextArgs);
  float get_text_size(std::string_view);
  // text ops
};