#pragma once

#include "runner.h"

namespace week2
{
    long day10a();
    long day10b();

    const runner::method_map_t method_map =
    {
        { "10a", &day10a },
        { "10b", &day10b },
    };
};
