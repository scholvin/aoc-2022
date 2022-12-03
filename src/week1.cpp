#include "week1.h"
#include "util.h"

#include <fstream>
#include <set>
#include <utility>
#include <iostream>
#include <bitset>

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
    typedef std::pair<char, char> RPS_t;
    struct RPSParser
    {
        RPS_t operator()(const std::string& str)
        {
            return RPS_t{ str[0], str[2] };
        }
    };

    long day02a()
    {
        std::vector<RPS_t> games;
        readers::read_by_line("../data/day02.dat", RPSParser(), games);
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
        std::vector<RPS_t> games;
        readers::read_by_line("../data/day02.dat", RPSParser(), games);
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
};
