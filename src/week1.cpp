#include "week1.h"
#include "util.h"

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <set>
#include <utility>
#include <iostream>
#include <bitset>
#include <deque>

namespace week1
{
    long day01(char part)
    {
        std::ifstream infile("../data/day01.dat");
        std::string line;
        std::set<long> meals;
        long meal = 0;
        while (std::getline(infile, line))
        {
            if (line.size() == 0)
            {
                meals.insert(meal);
                meal = 0;
            }
            else
            {
                meal += std::stol(line);
            }

        }

        auto it = meals.rbegin();
        long result = *it;
        if (part == 'a')
            return result;
        result += *++it;
        result += *++it;

        return result;
    }

    // RPS == Rock Paper Scissors, opponent is first
    typedef std::pair<char, char> rps_t;
    struct rps_parser
    {
        rps_t operator()(const std::string& str)
        {
            return rps_t{ str[0], str[2] };
        }
    };

    long day02a()
    {
        std::vector<rps_t> games;
        readers::read_by_line("../data/day02.dat", rps_parser(), games);
        long score = 0;
        for (auto it: games)
        {
            long opp = it.first - 'A' + 1;
            long me = it.second - 'X' + 1;

            if (me - opp == 1 || me - opp == -2) // I win
                score += me + 6;
            else if (opp - me == 1 || opp - me == -2) // I lose
                score += me;
            else if (opp == me) // draw
                score += me + 3;
            else // bug
                abort();
        }

        return score;
    }

    long day02b()
    {
        std::vector<rps_t> games;
        readers::read_by_line("../data/day02.dat", rps_parser(), games);
        long score = 0;
        for (auto it: games)
        {
            long opp = it.first - 'A' + 1;
            long outcome = it.second - 'X' + 1;
            long me = 0;
            switch (outcome)
            {
                case 1: // I lose
                    me = opp - 1;
                    if (me == 0) me = 3;
                    score += me;
                    break;
                case 2: // draw
                    me = opp;
                    score += me + 3;
                    break;
                case 3: // I win
                    me = opp + 1;
                    if (me == 4) me = 1;
                    score += me + 6;
                    break;
            }
        }

        return score;
    }

    long day03a()
    {
        std::vector<std::string> rucksacks;
        readers::read_by_line("../data/day03.dat", rucksacks);
        long sum = 0;

        for (auto it: rucksacks)
        {
            for (size_t i = 0; i < it.length() / 2; i++)
            {
                for (size_t j = it.length() / 2; j < it.length(); j++)
                {
                    if (it[i] == it[j])
                    {
                        if (it[i] <= 'Z')
                            sum += it[i] - 'A' + 27;
                        else
                            sum += it[i] - 'a' + 1;
                        goto next;
                    }
                }
            }
// interesting: both compilers need that semi after the label, else they complain about a missing primary expreession
next: ;
        }
        return sum;
    }

    long day03b()
    {
        std::vector<std::string> rucksacks;
        readers::read_by_line("../data/day03.dat", rucksacks);
        long sum = 0;

        // convert each rucksack to a bitset, logically AND to find which position is set in each
        typedef std::bitset<52> bits_t;

        for (auto it = rucksacks.begin(); it != rucksacks.end(); )
        {
            bits_t bits[3];
            for (size_t i = 0; i < 3; i++)
            {
                for (auto c: *it)
                {
                    if (c <= 'Z')
                        bits[i].set(c - 'A' + 26);
                    else
                        bits[i].set(c - 'a');
                }
                it++;
            }
            bits_t result = bits[0] & bits[1] & bits[2];
            assert(result.count() == 1);
            for (size_t i = 0; i < 52; i++)
            {
                if (result[i])
                {
                    sum += i + 1;
                    break;
                }
            }
        }

        return sum;
    }

    typedef std::pair<long, long> range_t;
    typedef std::pair<range_t, range_t> range_pair_t;
    struct range_pair_parser
    {
        range_pair_t operator()(const std::string& str)
        {
            size_t c = str.find(',');
            size_t d1 = str.find('-');
            size_t d2 = str.find('-', c);
            return range_pair_t{
                { std::stol(str.substr(0, d1)), std::stol(str.substr(d1+1, c-d1)) },
                { std::stol(str.substr(c+1, d2-c)), std::stol(str.substr(d2+1)) }
            };
        }
    };

    long day04a()
    {
        std::vector<range_pair_t> ranges;
        readers::read_by_line("../data/day04.dat", range_pair_parser(), ranges);

        long overlaps = 0;
        for (auto r: ranges)
        {
            if ((r.first.first <= r.second.first && r.first.second >= r.second.second) ||
                (r.second.first <= r.first.first && r.second.second >= r.first.second))
            {
                overlaps++;
            }
        }

        return overlaps;
    }

    long day04b()
    {
        std::vector<range_pair_t> ranges;
        readers::read_by_line("../data/day04.dat", range_pair_parser(), ranges);

        long overlaps = 0;
        for (auto r: ranges)
        {
            if ((r.second.first >= r.first.first && r.second.first <= r.first.second) ||
                (r.second.second >= r.first.first && r.second.second <= r.first.second) ||
                (r.first.first >= r.second.first && r.first.first <= r.second.second) ||
                (r.first.second >= r.second.first && r.first.second <= r.second.second))
            {
                overlaps++;
            }
        }

        return overlaps;
    }

    long day05(char part)
    {
        // use a deque, so we can load it from the back as we read the input top down, but push/pop from the front
        typedef std::deque<char> stack_t;
        std::vector<stack_t> stacks;
        std::ifstream infile("../data/day05.dat");
        std::string line;

        while (std::getline(infile, line))
        {
            if (stacks.size() == 0)
            {
                stacks.resize(line.length() / 4 + 1);
            }
            if (line.substr(0, 2) == " 1")
            {
                break;
            }
            for (size_t i = 0; i < stacks.size(); i++)
            {
                if (line[i*4 + 1] != ' ')
                {
                    stacks[i].push_back(line[i*4+1]);
                }
            }
        }
        std::getline(infile, line); // blank line
        while (std::getline(infile, line))
        {
            // "move X from Y to Z"
            std::vector<std::string> parts;
            boost::split(parts, line, boost::is_any_of(" "));
            long num = std::stol(parts[1]);
            long src = std::stol(parts[3]) - 1;
            long dest = std::stol(parts[5]) - 1;

            if (part == 'a')
            {
                for (auto i = 0; i < num; i++)
                {
                    stacks[dest].push_front(stacks[src].front());
                    stacks[src].pop_front();
                }
            }
            else // part b, of course
            {
                stack_t temp;
                for (auto i = 0; i < num; i++)
                {
                    temp.push_front(stacks[src].front());
                    stacks[src].pop_front();
                }
                for (auto i = 0; i < num; i++)
                {
                    stacks[dest].push_front(temp.front());
                    temp.pop_front();
                }
            }
        }

        for (auto it: stacks)
        {
            std::cout << it.front();
        }
        std::cout << std::endl;

        return 0;
    }
};
