#pragma once

#include "runner.h"

namespace week1
{
    long day01(char part);
    long day02a();
    long day02b();

    const runner::method_map_t method_map =
    {
        { "1a", std::bind(&day01, 'a') },
        { "1b", std::bind(&day01, 'b') },
        { "2a", std::bind(&day02a) },
        { "2b", std::bind(&day02b) },
        //{ "4a", std::bind(&day04, 'a') },
    };
};
