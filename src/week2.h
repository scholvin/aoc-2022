#pragma once

#include "runner.h"

namespace week2
{
    long day08a();
    long day10a();
    long day10b();
    long day11a();
    long day11b();

    const runner::method_map_t method_map =
    {
        { "8a", &day08a },
        { "10a", &day10a },
        { "10b", &day10b },
        { "11a", &day11a },
        { "11b", &day11b },
    };
};
