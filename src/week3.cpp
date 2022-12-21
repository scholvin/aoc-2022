#include "week3.h"
#include "util.h"

#include <boost/algorithm/string.hpp>

#include <vector>
#include <iostream>
#include <cstdio>

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

        typedef std::pair<long, long> range_t;
        std::vector<range_t> ranges;
        for (auto s: sensors)
        {
            // for each sensor, see if Y is in its radius, and exclude appropriate X range in that row
            if (Y >= s.y - s.dist && Y <= s.y + s.dist)
            {
                long delta = abs(Y - s.y);
                long x0 = s.x - (s.dist - delta);
                long x1 = s.x + (s.dist - delta);

                // std::cout << "sensor " << s.x << "," << s.y << " d=" << s.dist << " in range" << std::endl;
                //           << "  delta=" << delta << ": " << x0 << "," << x1 << std::endl;

                ranges.push_back(std::make_pair(x0, x1));
            }
        }

        // merge overlapping intervals? this might be fun - and it is, like 10000x faster
        std::sort(ranges.begin(), ranges.end(), [](range_t a, range_t b) { return a.first < b.first; });
        std::vector<range_t> merged;
        auto it = ranges.begin();
        merged.push_back(*it++);
        while (it != ranges.end())
        {
            // if *it overlaps with top of stack, merge them and repush, else push *it
            if (it->first <= merged.back().second)
            {
                merged.back().second = std::max(it->second, merged.back().second);
            }
            else
            {
                merged.push_back(*it);
            }
            it++;
        }

        long exclusions = 0;
        for (auto m: merged)
        {
            exclusions += m.second - m.first;
        }
        return exclusions;
    }
    //  4861076    12390055µs std::set
    //  4861076     3216357µs std::unordered_set
    //  4861076         230µs range merge!
};
