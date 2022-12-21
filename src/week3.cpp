#include "week3.h"
#include "util.h"

#include <boost/algorithm/string.hpp>

#include <vector>
#include <iostream>
#include <cstdio>
#include <set>

namespace week3
{
    struct sensor
    {
        long x, y;
        long dist;
    };

    struct beacon
    {
        long x, y;
    };

    long day15a()
    {
        std::vector<sensor> sensors;
        std::vector<beacon> beacons;

#if 0
        const long Y = 10;
        std::ifstream infile("../data/day15-smol.dat");
#else
        const long Y = 2000000;
        std::ifstream infile("../data/day15.dat");
#endif
        std::string line;

        long x_max = std::numeric_limits<long>::min();
        long y_max = std::numeric_limits<long>::min();
        long x_min = std::numeric_limits<long>::max();
        long y_min = std::numeric_limits<long>::max();

        while (std::getline(infile, line))
        {
            long xs, ys, xb, yb, dist;
            // enough of the parsing silliness, do something dirty
            assert(std::sscanf(line.c_str(), "Sensor at x=%ld, y=%ld: closest beacon is at x=%ld, y=%ld", &xs, &ys, &xb, &yb) == 4);
            dist = abs(xs-xb) + abs(ys-yb);
            sensors.push_back( { xs, ys, dist } );
            beacons.push_back( { xb, yb } );
            if (xs > x_max) x_max = xs;
            if (ys > y_max) y_max = ys;
            if (xb > x_max) x_max = xb;
            if (yb > y_max) y_max = yb;
            if (xs < x_min) x_min = xs;
            if (ys < y_min) y_min = ys;
            if (xb < x_min) x_min = xb;
            if (yb < y_min) y_min = yb;
        }

        std::set<long> excluded;
        for (auto s: sensors)
        {
            // for each sensor, see if Y is in its radius, and exclude needed X positions in that row
            // TODO, this is a very slow implementation, maybe better so save the ranges
            if (Y >= s.y - s.dist && Y <= s.y + s.dist)
            {
                std::cout << "sensor " << s.x << "," << s.y << " d=" << s.dist << " in range" << std::endl;

                long delta = abs(Y - s.y);
                long x0 = s.x - (s.dist - delta);
                long x1 = s.x + (s.dist - delta);
                std::cout << "  delta=" << delta << ": " << x0 << "," << x1 << std::endl;


                for (long x = x0; x <= x1; x++)
                {
                    excluded.insert(x);
                }
            }
        }

        // remove any beacons in the row
        for (auto b: beacons)
        {
            if (b.y == Y)
            {
                excluded.erase(b.x);
            }
        }
        return excluded.size();
    }
   //  4861076    12390055µs
};
