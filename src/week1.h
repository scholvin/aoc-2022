#pragma once

#include "runner.h"

namespace week1
{
    long day01(char part);
    long day02a();
    long day02b();
    long day03a();
    long day03b();
    long day04a();
    long day04b();
    long day05(char part);
    long day06a();
    long day06b();
    long day07(char part);

    const runner::method_map_t method_map =
    {
        { "1a", std::bind(&day01, 'a') },
        { "1b", std::bind(&day01, 'b') },
        { "2a", &day02a },
        { "2b", &day02b },
        { "3a", &day03a },
        { "3b", &day03b },
        { "4a", &day04a },
        { "4b", &day04b },
        { "5a", std::bind(&day05, 'a') },
        { "5b", std::bind(&day05, 'b') },
        { "6a", &day06a },
        { "6b", &day06b },
        { "7a", std::bind(&day07, 'a') },
        { "7b", std::bind(&day07, 'b') },
    };
};
