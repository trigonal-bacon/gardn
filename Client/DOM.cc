#include <Client/DOM.hh>

#include <Shared/Helpers.hh>

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
        document.body.appendChild(elem);
    },
    name, max_len * 2);
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
        const arr = new TextEncoder().encode(elem.value);
        const len = arr.length;
        // remember off by one errors
        const ptr = Module["_malloc"](len + 1);
        HEAPU8.set(arr, ptr);
        HEAPU8[ptr + len] = 0;
        return ptr;
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