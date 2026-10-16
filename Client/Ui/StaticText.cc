#include <Client/Ui/StaticText.hh>

#include <Client/Render/Renderer.hh>

#include <Helpers/Macros.hh>

#include <vector>

using namespace Ui;

StaticText::StaticText(float font_size, std::string_view str, Style s)
    : Element(Renderer::get_ascii_text_size(str) * font_size, font_size, s),
      text(str) {}

void StaticText::on_render(Renderer& ctx) {
  ctx.draw_text(text.c_str(),
                {.fill = style.fill, .size = height, .stroke_scale = 0.12});
}

static std::vector<std::string_view> _split_by_spaces(std::string_view str) {
  std::vector<std::string_view> vec;
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
  if (last < at)
    vec.push_back(str.substr(last, at - last));
  return vec;
}

StaticParagraph::StaticParagraph(float max_width, float text_height,
                                 std::string_view str, Style s)
    : VContainer({}, 0, text_height / 4, s) {
  std::vector<std::string_view> words = _split_by_spaces(str);
  DEBUG_ONLY(assert(str.size() > 0);)
  std::string line = std::string(words[0]);
  size_t last = 0;
  size_t at = 1;
  size_t len = words.size();
  while (at < len) {
    std::string curr = std::string(words[at]);
    if (Renderer::get_ascii_text_size((line + " " + curr).c_str()) *
            text_height >=
        max_width) {
      add_child(el<StaticText>(text_height, line,
                               Style{.fill = 0xffffffff,
                                     .h_justify = Style::Left,
                                     .v_justify = Style::Top}));
      line = std::move(curr);
    } else
      line += ' ' + curr;
    ++at;
  }
  if (line != "")
    add_child(el<StaticText>(text_height, line,
                             Style{.fill = 0xffffffff,
                                   .h_justify = Style::Left,
                                   .v_justify = Style::Top}));
  refactor();
}