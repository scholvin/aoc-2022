#include "week1.h"
#include "util.h"

#include <fstream>
#include <set>
#include <utility>

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
    typedef std::pair<char, char> RPS;
    struct RPSParser
    {
        RPS operator()(const std::string& str)
        {
            return RPS{ str[0], str[2] };
        }

    };

    long day02a()
    {
        std::vector<RPS> games;
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
        std::vector<RPS> games;
        readers::read_by_line("../data/day02.dat", RPSParser(), games);
        long score = 0;
        for (auto it: games)
        {
            long opp = it.first - 'A' + 1;
            long outcome = it.second - 'X' + 1;
            long me = 0;
            switch (outcome)
            {
                case 1: /* I lose */
                    me = opp - 1;
                    if (me == 0) me = 3;
                    score += me;
                    break;
                case 2: /* draw */
                    me = opp;
                    score += me + 3;
                    break;
                case 3: /* I win */
                    me = opp + 1;
                    if (me == 4) me = 1;
                    score += me + 6;
                    break;
            }
        }

        return score;
    }

};
