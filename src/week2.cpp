#include "week2.h"
#include "util.h"

#include <vector>
#include <iostream>

namespace week2
{
    long day10a()
    {
        std::vector<std::string> instructions;
        readers::read_by_line("../data/day10.dat", instructions);

        long cycle = 1;
        long x = 1;
        long sum = 0;

        auto check = [&]() -> bool
        {
            // std::cout << "cycle=" << cycle << " x=" << x << std::endl;
            return (cycle == 20 || (cycle >= 60 && (cycle - 20) % 40 == 0));
        };

        for (auto inst: instructions)
        {
            if (inst[0] == 'n') // noop
            {
                cycle++;
                if (check()) sum += cycle * x;
            }
            else // addx op
            {
                long op = std::stol(inst.substr(inst.find(' ') + 1));
                cycle++;
                if (check()) sum += cycle * x;
                x += op;
                cycle++;
                if (check()) sum += cycle * x;
            }
        }
        return sum;
    }

    long day10b()
    {
        std::vector<std::string> instructions;
        readers::read_by_line("../data/day10.dat", instructions);

        long cycle = 0;
        long x = 1;

        auto pixel = [&]()
        {
            long mod = cycle % 40;
            if (cycle > 0 && mod == 0)
            {
                std::cout << std::endl;
            }
            if (cycle == 240)
            {
                return;
            }
            std::cout << ((mod >= x-1 && mod <= x+1) ? "▓" : " ");
        };

        pixel();
        for (auto inst: instructions)
        {
            if (inst[0] == 'n') // noop
            {
                cycle++;
            }
            else // addx op
            {
                long op = std::stol(inst.substr(inst.find(' ') + 1));
                cycle++;
                pixel();
                cycle++;
                x += op;
            }
            pixel();
        }

        return 0; // cout gets RBPARAGF if you squint
    }
};
