#include <Client/Ui/StaticText.hh>

#include <Client/Render/Renderer.hh>

#include <Helpers/Macros.hh>

#include <vector>

using namespace Ui;

StaticText::StaticText(float font_size, std::string const &str, Style s) : 
Element(Renderer::get_ascii_text_size(str.c_str()) * font_size, font_size, s), text(std::move(str)) {}

void StaticText::on_render(Renderer &ctx) {
    ctx.draw_text(text.c_str(), { .fill = style.fill, .size = height, .stroke_scale = 0.12 });
}

static std::vector<std::string> _split_by_spaces(std::string const &str) {
    std::vector<std::string> vec;
    size_t last = 0;
    size_t at = 0;
    size_t len = str.size();
    while (at < len) {
        if (str[at] == ' ') {
            vec.push_back(str.substr(last, at - last));
            last = at + 1;
        }
        ++at;
    }
    if (last < at) vec.push_back(str.substr(last, at - last));
    return vec;
}

StaticParagraph::StaticParagraph(float max_width, float text_height, std::string str, Style s) : VContainer({}, 0, text_height / 4, s) {
    std::vector<std::string> words = _split_by_spaces(str);
    DEBUG_ONLY(assert(str.size() > 0);)
    std::string line = words[0];
    size_t last = 0;
    size_t at = 1;
    size_t len = words.size();
    while (at < len) {
        if (Renderer::get_ascii_text_size((line + " " + words[at]).c_str()) * text_height >= max_width) {
            add_child(new Ui::StaticText(text_height, line, { .fill = 0xffffffff, .h_justify = Style::Left, .v_justify = Style::Top }));
            line = words[at];
        } else 
            line += ' ' + words[at];
        ++at;
    }
    if (line != "")
        add_child(new Ui::StaticText(text_height, line, { .fill = 0xffffffff, .h_justify = Style::Left, .v_justify = Style::Top }));
    refactor();
}