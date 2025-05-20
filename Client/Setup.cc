#include <Client/Setup.hh>
#include <emscripten.h>

int setup_canvas() {
    EM_ASM({
        Module.canvas = document.createElement("canvas");
        Module.canvas.id = "canvas";
        Module.canvas.width = innerWidth * devicePixelRatio;
        Module.canvas.height = innerHeight * devicePixelRatio;
        Module.canvas.oncontextmenu = function() { return false; };
        window.onbeforeunload = function(e) { return "Are you sure?"; };
        document.body.appendChild(Module.canvas);
        Module.ctxs = [Module.canvas.getContext('2d')];
        Module.availableCtxs =
            new Array(256).fill(0).map(function(_, i) { return i; });
        Module.TextDecoder = new TextDecoder('utf8');
    });
    return 0;
}

int setup_inputs() {
    EM_ASM({
        window.addEventListener("keydown", (e) => {
            _key_event(e.which, 0);
        });
        window.addEventListener("keyup", (e) => {
            _key_event(e.which, 1);
        });
        window.addEventListener("mousedown", (e) => {
            _mouse_event(e.clientX * devicePixelRatio, e.clientY * devicePixelRatio, 0, +!!e.button);
        });
        window.addEventListener("mousemove", (e) => {
            _mouse_event(e.clientX * devicePixelRatio, e.clientY * devicePixelRatio, 1, +!!e.button);
        });
        window.addEventListener("mouseup", (e) => {
            _mouse_event(e.clientX * devicePixelRatio, e.clientY * devicePixelRatio, 2, +!!e.button);
        });
    });
    return 0;
}

void main_loop() {
    EM_ASM({
        function loop(time)
        {
            Module.canvas.width = innerWidth * devicePixelRatio;
            Module.canvas.height = innerHeight * devicePixelRatio;
            _loop(time, Module.canvas.width, Module.canvas.height);
            requestAnimationFrame(loop);
        };
        requestAnimationFrame(loop);
    });
}