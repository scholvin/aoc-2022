#include "week2.h"
#include "util.h"

#include <boost/algorithm/string.hpp>

#include <vector>
#include <iostream>

namespace week2
{
    struct tree_t
    {
        int8_t height;
        bool seen;
    };

    struct tree_parser
    {
        tree_t operator()(char c) { return { static_cast<int8_t>(c - '0'), false }; }
    };

    // cheated here and looked at the data file to avoid dynamic array shenanigans
    const int F_WIDTH = 99;
    const int F_HEIGHT = 99;

    typedef std::array<std::array<tree_t, F_HEIGHT>, F_WIDTH> forest_t;

    void vis08(const forest_t& forest)
    {
        const std::string RED = "[31m";
        const std::string GREEN = "[32m";
        for (auto y = 0; y < F_HEIGHT; y++)
        {
            for (auto x = 0; x < F_WIDTH; x++)
            {
                std::cout << (forest[x][y].seen ? GREEN : RED) << static_cast<char>(forest[x][y].height + '0');
            }
            std::cout << std::endl;
        }
    }

    long day08a()
    {
        forest_t forest;
        readers::read_dense_2d_matrix("../data/day08.dat", tree_parser(), forest);

        long visible = 0;
        // from the left
        for (auto y = 0; y < F_HEIGHT; y++)
        {
            int8_t max = -1;
            for (auto x = 0; x < F_WIDTH; x++)
            {
                if (forest[x][y].height > max)
                {
                    if (!forest[x][y].seen)
                    {
                        visible++;
                        forest[x][y].seen = true;
                    }
                    max = forest[x][y].height;
                }
            }
        }
#ifdef VISUALIZE
        std::cout << "after left " << visible << std::endl;
        vis08(forest);
#endif
        // from the right
        for (auto y = 0; y < F_HEIGHT; y++)
        {
            int8_t max = -1;
            for (auto x = F_WIDTH-1; x >= 0; x--)
            {
                if (forest[x][y].height > max)
                {
                    if (!forest[x][y].seen)
                    {
                        visible++;
                        forest[x][y].seen = true;
                    }
                    max = forest[x][y].height;
                }
            }
        }
#ifdef VISUALIZE
        std::cout << "after right " << visible << std::endl;
        vis08(forest);
#endif
        // from the top
        for (auto x = 0; x < F_WIDTH; x++)
        {
            int8_t max = -1;
            for (auto y = 0; y < F_HEIGHT; y++)
            {
                if (forest[x][y].height > max)
                {
                    if (!forest[x][y].seen)
                    {
                        visible++;
                        forest[x][y].seen = true;
                    }
                    max = forest[x][y].height;
                }
            }
        }
#ifdef VISUALZE
        std::cout << "after top " << visible << std::endl;
        vis08(forest);
#endif
        // from the bottom
        for (auto x = 0; x < F_WIDTH; x++)
        {
            int8_t max = -1;
            for (auto y = F_HEIGHT-1; y >= 0; y--)
            {
                if (forest[x][y].height > max)
                {
                    if (!forest[x][y].seen)
                    {
                        visible++;
                        forest[x][y].seen = true;
                    }
                    max = forest[x][y].height;
                }
            }
        }
#ifdef VISUALIZE
        std::cout << "after bottom " << visible << std::endl;
        vis08(forest);
#endif
        return visible;
    }

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

    enum monkey_op
    {
        ADD,
        MULT,
        SQUARE
    };

    struct monkey
    {
        std::deque<long> items;
        monkey_op op;
        long operand;
        long divisor;
        size_t dest_true;
        size_t dest_false;
        long inspections;
    };

    struct monkey_parser
    {
        monkey operator()(const std::vector<std::string>& lines)
        {

        /*
            example:

            Monkey 2:
              Starting items: 79, 60, 97
              Operation: new = old * old
              Test: divisible by 13
                If true: throw to monkey 1
                If false: throw to monkey 3
        */

            // this is the most aggravating parser yet, thanks, AoC guy
            std::vector<std::string> words;
            std::string foo = lines[1].substr(18); // boost split doesn't like a non-const lvalue?
            boost::algorithm::split(words, foo, boost::is_any_of(", "), boost::token_compress_on);
            std::deque<long> items;
            for (auto w: words)
                items.push_back(boost::lexical_cast<long>(w));

            monkey_op op = SQUARE;
            long operand = 1;
            if (lines[2][23] == '+')
            {
                op = ADD;
                operand = boost::lexical_cast<long>(lines[2].substr(25));
            }
            else
            {
                if (lines[2][25] != 'o')
                {
                    op = MULT;
                    operand = boost::lexical_cast<long>(lines[2].substr(25));
                }
            }

            long divisor = boost::lexical_cast<long>(lines[3].substr(21));
            size_t dest_true = boost::lexical_cast<size_t>(lines[4].substr(29));
            size_t dest_false = boost::lexical_cast<size_t>(lines[5].substr(30));

            // TODO, benchmark std::stol vs boost::lexical_cast

            return monkey {
                { items },
                op,
                operand,
                divisor,
                dest_true,
                dest_false,
                0
            };
        }
    };

    long day11a()
    {
        std::vector<monkey> monkeys;
        readers::read_by_n_lines("../data/day11.dat", 7, monkey_parser(), monkeys);

        for (int r = 0; r < 20; r++)
        {
            for (auto& m: monkeys)
            {
                for (auto it = m.items.begin(); it != m.items.end(); )
                {
                    m.inspections++;
                    switch (m.op)
                    {
                        case ADD: *it += m.operand; break;
                        case MULT: *it *= m.operand; break;
                        case SQUARE: *it *= *it; break;
                    }
                    *it /= 3;
                    if (*it % m.divisor == 0)
                    {
                        monkeys[m.dest_true].items.push_back(*it);
                    }
                    else
                    {
                        monkeys[m.dest_false].items.push_back(*it);
                    }
                    it++;
                    m.items.pop_front();
                }
            }
        }

        // find the two most active monkeys
        std::sort(monkeys.begin(), monkeys.end(), [](monkey& a, monkey& b) { return a.inspections > b.inspections; } );
        return monkeys[0].inspections * monkeys[1].inspections;
    }

    long day11b()
    {
        std::vector<monkey> monkeys;
        readers::read_by_n_lines("../data/day11.dat", 7, monkey_parser(), monkeys);

        long master = 1;
        for (auto m: monkeys)
        {
            // had to look this trick up, not gonna lie
            master *= m.divisor;
        }

        for (int r = 0; r < 10000; r++)
        {
            for (auto& m: monkeys)
            {
                for (auto it = m.items.begin(); it != m.items.end(); )
                {
                    m.inspections++;
                    switch (m.op)
                    {
                        case ADD: *it += m.operand; break;
                        case MULT: *it *= m.operand; break;
                        case SQUARE: *it *= *it; break;
                    }
                    *it %= master;
                    if (*it % m.divisor == 0)
                    {
                        monkeys[m.dest_true].items.push_back(*it);
                    }
                    else
                    {
                        monkeys[m.dest_false].items.push_back(*it);
                    }
                    it++;
                    m.items.pop_front();
                }
            }
        }

        std::sort(monkeys.begin(), monkeys.end(), [](monkey& a, monkey& b) { return a.inspections > b.inspections; } );
        return monkeys[0].inspections * monkeys[1].inspections;
    }

};
