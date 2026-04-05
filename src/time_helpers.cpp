#include "time_helpers.h"

float Time::DeltaTime() {
    using clock = std::chrono::high_resolution_clock;
    static auto last = clock::now();
    auto now = clock::now();
    std::chrono::duration<float> dt = now - last;
    last = now;
    return dt.count();
}