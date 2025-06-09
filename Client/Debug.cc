#include <Client/Debug.hh>

#include <emscripten.h>

namespace Debug {
    CircularArray<double, 120> tick_times;
    CircularArray<double, 120> frame_times;
}

double Debug::get_timestamp() {
    return EM_ASM_DOUBLE({ return performance.now(); });
}