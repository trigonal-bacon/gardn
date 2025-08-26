#pragma once

#include <Helpers/Array.hh>

namespace Debug {
    extern CircularArray<double, 100> tick_times;
    extern CircularArray<double, 100> frame_times;
    extern CircularArray<double, 50> ping_times;

    double get_timestamp();
}