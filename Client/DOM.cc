#include <Client/DOM.hh>

#include <Helpers/UTF8.hh>

#include <emscripten.h>

void DOM::create_text_input(std::string_view name, uint32_t max_len) {
    EM_ASM({
        const name = UTF8ToString($0, $1);
        const elem = document.createElement('input');
        elem.id = name;
        elem.type = "text";
        elem.style.position = "absolute";
        elem.style["font-family"] = "Ubuntu";
        elem.style.display = 'none';
        elem.style["border-width"] = "0px";
        elem.style.background = "transparent";
        elem.style.border = "none";
        elem.style.outline = "none";
        elem.style["padding-left"] = "5px";
        elem.maxLength = ($2).toString();
        elem.setAttribute("spellcheck", "false");
        document.body.appendChild(elem);
    }, name.data(), name.size(), max_len * 2);
}

void DOM::element_show(std::string_view name)
{
    EM_ASM({
        const name = UTF8ToString($0, $1);
        const elem = document.getElementById(name);
        elem.style.display = 'block';
    }, name.data(), name.size());
}

void DOM::element_hide(std::string_view name)
{
    EM_ASM({
        const name = UTF8ToString($0, $1);
        const elem = document.getElementById(name);
        elem.style.display = 'none';
    }, name.data(), name.size());
}

void DOM::update_pos_and_dimension(std::string_view name, float x, float y, float w, float h)
{
    EM_ASM({
        const name = UTF8ToString($0, $1);
        const elem = document.getElementById(name);
        elem.style.left = ($2 - $4 / 2) / devicePixelRatio + "px";
        elem.style.top = ($3 - $5 / 2) / devicePixelRatio + "px";
        elem.style.width = ($4 / devicePixelRatio - 10) + "px";
        elem.style.height = $5 / devicePixelRatio + "px";
        elem.style["font-size"] = $5 / devicePixelRatio * 0.66 + "px";
    }, name.data(), name.size(), x, y, w, h);
}

std::string DOM::retrieve_text(std::string_view name, uint32_t max_length) {
    char *ptr = (char *) EM_ASM_PTR({
        const name = UTF8ToString($0, $1);
        const elem = document.getElementById(name);
        return stringToNewUTF8(elem.value);
    }, name.data(), name.size());
    std::string out{ptr};
    out = UTF8Parser::trunc_string(out, max_length);
    free(ptr);
    return out;
}

void DOM::update_text(std::string_view name, std::string_view contents, uint32_t max_length) {
    EM_ASM({
        const name = UTF8ToString($0, $1);
        const elem = document.getElementById(name);
        elem.value = UTF8ToString($2, $3).slice(0, $4);
    }, name.data(), name.size(), contents.data(), contents.size(), max_length);
}

void DOM::open_page(std::string_view url) {
    EM_ASM({ 
        try {
            window.open(UTF8ToString($0, $1));
        } catch(e) {}
    }, url.data(), url.size());
}