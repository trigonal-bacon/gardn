#include <Client/Debug.hh>

#include <emscripten.h>

namespace Debug {
    CircularArray<double, 100> tick_times;
    CircularArray<double, 100> frame_times;
    CircularArray<double, 50> ping_times;
}

double Debug::get_timestamp() {
    return EM_ASM_DOUBLE({ return performance.now(); });
}