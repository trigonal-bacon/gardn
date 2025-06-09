#pragma once

#include <Shared/Helpers.hh>

namespace Debug {
    extern CircularArray<double, 120> tick_times;
    extern CircularArray<double, 120> frame_times;

    double get_timestamp();
}