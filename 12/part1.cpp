// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using Pos = std::pair<int, int>;

int main()
{
    std::string s;
    std::vector<std::string> heightmap;
    Pos start, end;
    while (std::getline(std::cin, s))
    {
        auto const sit = std::find(s.begin(), s.end(), 'S');
        if (sit != s.end())
        {
            start = {static_cast<int>(heightmap.size()),
                     static_cast<int>(std::distance(s.begin(), sit))};
            *sit = 'a';
        }

        auto const eit = std::find(s.begin(), s.end(), 'E');
        if (eit != s.end())
        {
            end = {static_cast<int>(heightmap.size()),
                   static_cast<int>(std::distance(s.begin(), eit))};
            *eit = 'z';
        }

        heightmap.push_back(s);
    }

    int numSteps = 0;
    std::set<Pos> taken;
    std::vector<Pos> next{start};

    auto const isValidMove = [&](int fromr, int fromc, int tor, int toc) {
        if (tor < 0 || tor >= heightmap.size() || toc < 0
            || toc >= heightmap.front().size())
            return false;

        char const from = heightmap[fromr][fromc];
        char const to = heightmap[tor][toc];
        return to <= from + 1;
    };

    while (true)
    {
        std::vector<Pos> const localNext = next;
        next.clear();

        for (Pos const& each : localNext)
        {
            if (each == end)
            {
                std::cout << numSteps << "\n";
                exit(EXIT_SUCCESS);
            }

            for (Pos const& delta : {Pos{-1, 0}, Pos{1, 0}, Pos{0, 1}, Pos{0, -1}})
            {
                Pos const to{each.first + delta.first, each.second + delta.second};
                if (isValidMove(each.first, each.second, to.first, to.second))
                {
                    if (taken.count(to) == 0)
                    {
                        taken.insert(to);
                        next.push_back(to);
                    }
                }
            }
        }

        ++numSteps;
    }
}
