#pragma once

#include "runner.h"

namespace week1
{
    long day01(char part);

    const runner::method_map_t method_map =
    {
        { "1a", std::bind(&day01, 'a') },
        { "1b", std::bind(&day01, 'b') },
        //{ "4a", std::bind(&day04, 'a') },
    };
};
