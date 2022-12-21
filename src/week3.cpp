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

    void read_day_15(const std::string& filename,
                     std::vector<sensor>& sensors, std::vector<beacon>& beacons,
                     long& x_max, long& y_max, long& x_min, long& y_min)
    {
        x_max = std::numeric_limits<long>::min();
        y_max = std::numeric_limits<long>::min();
        x_min = std::numeric_limits<long>::max();
        y_min = std::numeric_limits<long>::max();

        std::ifstream infile(filename);
        std::string line;
        while (std::getline(infile, line))
        {
            long xs, ys, xb, yb, dist;
            // enough of the parsing silliness, do something dirty
            int r = std::sscanf(line.c_str(), "Sensor at x=%ld, y=%ld: closest beacon is at x=%ld, y=%ld", &xs, &ys, &xb, &yb);
            assert(r == 4);
            (void)r; // keep optimized compile quiet
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
    }

    long day15a()
    {
        std::vector<sensor> sensors;
        std::vector<beacon> beacons;
        long x_max;
        long y_max;
        long x_min;
        long y_min;

#if 0
        const long Y = 10;
        read_day_15("../data/day15-smol.dat", sensors, beacons, x_max, y_max, x_min, y_min);
#else
        const long Y = 2000000;
        read_day_15("../data/day15.dat", sensors, beacons, x_max, y_max, x_min, y_min);
#endif

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

        // merge overlapping intervals? this might be fun - and it is, like 6 orders of magnitude faster
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

    long day15b()
    {
        std::vector<sensor> sensors;
        std::vector<beacon> beacons;
        long x_max;
        long y_max;
        long x_min;
        long y_min;

#if 0
        read_day_15("../data/day15-smol.dat", sensors, beacons, x_max, y_max, x_min, y_min);
        const long X_MAX = 14;
        const long Y_MAX = 11;
#else
        const long X_MAX = 4000000;
        const long Y_MAX = X_MAX;
        read_day_15("../data/day15.dat", sensors, beacons, x_max, y_max, x_min, y_min);
#endif

        /* idea stolen from reddit, but not the implementation:

           for each sensor:
             iterate over each point x,y immediately outside its coverage perimeter
               if x,y is not in any other sensor's range, it's the answer

            this will be something like O(s^2) but maybe that's OK (it kinda has to be)
        */

        auto covers = [](const sensor& s, long x, long y) -> bool
        {
            return (abs(x-s.x) + abs(y-s.y) <= s.dist);
        };

        for (size_t s = 0; s < sensors.size(); s++)
        {
            const sensor& S = sensors[s];
            long x, y, d;
            bool found;

            // from top to right
            for (d = 0, x = S.x, y = S.y + S.dist + 1; d < S.dist + 1; d++, x++, y--)
            {
                if (x < 0 || y < 0 || x > X_MAX || y > Y_MAX)
                {
                    continue;
                }
                found = false;
                for (size_t t = s + 1; t < sensors.size(); t++)
                {
                    const sensor& T = sensors[t];
                    if (covers(T, x, y))
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    return x * 4000000 + y;
                }
            }

            // from right to bottom
            for (d = 0; d < S.dist + 1; d++, x--, y--)
            {
                if (x < 0 || y < 0 || x > X_MAX || y > Y_MAX)
                {
                    continue;
                }
                found = false;
                for (size_t t = s + 1; t < sensors.size(); t++)
                {
                    const sensor& T = sensors[t];
                    if (covers(T, x, y))
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    return x * 4000000 + y;
                }
            }

            // from bottom to left
            for (d = 0; d < S.dist + 1; d++, x--, y++)
            {
                if (x < 0 || y < 0 || x > X_MAX || y > Y_MAX)
                {
                    continue;
                }
                found = false;
                for (size_t t = s + 1; t < sensors.size(); t++)
                {
                    const sensor& T = sensors[t];
                    if (covers(T, x, y))
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    return x * 4000000 + y;
                }
            }

            // from left to top
            for (d = 0; d < S.dist + 1; d++, x++, y++)
            {
                if (x < 0 || y < 0 || x > X_MAX || y > Y_MAX)
                {
                    continue;
                }
                found = false;
                for (size_t t = s + 1; t < sensors.size(); t++)
                {
                    const sensor& T = sensors[t];
                    if (covers(T, x, y))
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    return x * 4000000 + y;
                }
            }
        }

        return -1; // error
    }
};
