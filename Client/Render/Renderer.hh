#pragma once

#include <stdint.h>

class Renderer;

class RenderContext {
    Renderer *renderer;
public:
    float transform_matrix[6] = {0};
    uint32_t color_filter = 0xff000000;
    float amount = 0;
    RenderContext(Renderer *);
    void reset();
    ~RenderContext();
};

class Renderer {
public:
    RenderContext context;
    uint32_t id = 0;
    float width = 0;
    float height = 0;
    Renderer();
    ~Renderer();

    static uint32_t HSV(uint32_t, float);
    static uint32_t MIX(uint32_t, uint32_t, float);

    void add_color_filter(uint32_t, float);
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
    void fill();
    void stroke();
    void clip();

    void fill_text(char const *);
    void stroke_text(char const *);
    float get_text_size(char const *);
    //text ops
};