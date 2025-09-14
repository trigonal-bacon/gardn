#include <Client/DOM.hh>

#include <Client/Input.hh>

#include <Helpers/UTF8.hh>

#include <emscripten.h>

void DOM::create_text_input(char const *name, uint32_t max_len) {
    EM_ASM(
    {
        const name = UTF8ToString($0);
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
        elem.maxLength = ($1).toString();
        elem.setAttribute("spellcheck", "false");
        if ($2) {
            elem.addEventListener("keydown", e => {
                if (e.key === "Enter" && !e.repeat) {
                    e.stopPropagation();
                    _key_event(stringToNewUTF8(e.key), 0);
                    elem.blur();
                }
            });
            elem.addEventListener("keyup", e => {
                if (e.key === "Enter" && !e.repeat) {
                    e.stopPropagation();
                    _key_event(stringToNewUTF8(e.key), 1);
                }
            });
            elem.addEventListener("blur", () => {
                _blur_event(1);
            });
        }
        document.body.appendChild(elem);
    },
    name, max_len, Input::is_mobile);
}

void DOM::element_show(char const *name)
{
    EM_ASM(
    {
        const name = UTF8ToString($0);
        const elem = document.getElementById(name);
        elem.style.display = 'block';
    },
    name);
}

void DOM::element_hide(char const *name)
{
    EM_ASM(
    {
        const name = UTF8ToString($0);
        const elem = document.getElementById(name);
        elem.style.display = 'none';
    },
    name);
}

void DOM::update_pos_and_dimension(char const *name, float x, float y, float w, float h)
{
    EM_ASM(
        {
            const name = UTF8ToString($0);
            const elem = document.getElementById(name);
            elem.style.left = ($1 - $3 / 2) / devicePixelRatio + "px";
            elem.style.top = ($2 - $4 / 2) / devicePixelRatio + "px";
            elem.style.width = ($3 / devicePixelRatio - 10) + "px";
            elem.style.height = $4 / devicePixelRatio + "px";
            elem.style["font-size"] = $4 / devicePixelRatio * 0.66 + "px";
        },
        name, x, y, w, h);
}

std::string DOM::retrieve_text(char const *name, uint32_t max_length) {
    char *ptr = (char *) EM_ASM_PTR(
    {
        const name = UTF8ToString($0);
        const elem = document.getElementById(name);
        return stringToNewUTF8(elem.value);
    },
    name);
    std::string out{ptr};
    out = UTF8Parser::trunc_string(out, max_length);
    free(ptr);
    return out;
}

void DOM::update_text(char const *name, std::string const &contents, uint32_t max_length) {
    EM_ASM({
        const name = UTF8ToString($0);
        const elem = document.getElementById(name);
        elem.value = UTF8ToString($1).slice(0,$2);
    }, name, contents.c_str(), max_length);
}

void DOM::element_focus(char const *name) {
    EM_ASM({
        const name = UTF8ToString($0);
        const elem = document.getElementById(name);
        elem.focus();
    }, name);
}

void DOM::open_page(char const *url) {
    EM_ASM({
        try {
            window.open(UTF8ToString($0));
        } catch(e) {}
    }, url);
}

void DOM::reload_page() {
    EM_ASM({
        window.onbeforeunload = null;
        window.location.reload();
    });
}

void DOM::toggle_fullscreen() {
    EM_ASM({
        if (!document.fullscreenElement)
            document.documentElement.requestFullscreen();
        else
            document.exitFullscreen();
    });
}
