#pragma once

#include "runner.h"

namespace week1
{
    long day01(char part);
    long day02a();
    long day02b();
    long day03a();
    long day03b();


    const runner::method_map_t method_map =
    {
        { "1a", std::bind(&day01, 'a') },
        { "1b", std::bind(&day01, 'b') },
        { "2a", &day02a },
        { "2b", &day02b },
        { "3a", &day03a },
        { "3b", &day03b },
    };
};
