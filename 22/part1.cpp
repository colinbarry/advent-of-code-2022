// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

enum class Dir
{
    right,
    down,
    left,
    up,
};

int main()
{
    std::string s;
    std::vector<std::string> world;
    while (std::getline(std::cin, s))
    {
        if (s.size() == 0)
            break;
        world.push_back(s);
    }

    std::string instr;
    std::getline(std::cin, instr);

    int r = 0;
    int c = std::distance(world.front().cbegin(),
                          std::find(world.front().cbegin(), world.front().cend(), '.'));
    Dir dir = Dir::right;

    // Return the index of the cell in the given direction
    auto const cellIndex = [&](int r, int c, Dir const dir) -> std::pair<int, int> {
        switch (dir)
        {
            case Dir::up:
                --r;
                if (r < 0 || c >= world[r].size() || world[r][c] == ' ')
                {
                    r = std::distance(world.cbegin(),
                                      std::find_if(world.crbegin(),
                                                   world.crend(),
                                                   [&](auto const& row) {
                                                       return row.size() > c
                                                              && row[c] != ' ';
                                                   })
                                              .base()
                                          - 1);
                }
                break;

            case Dir::down:
                ++r;
                if (r >= world.size() || c >= world[r].size() || world[r][c] == ' ')
                {
                    r = std::distance(world.cbegin(),
                                      std::find_if(world.cbegin(),
                                                   world.cend(),
                                                   [&](auto const& row) {
                                                       return row.size() > c
                                                              && row[c] != ' ';
                                                   }));
                }
                break;

            case Dir::left:
                --c;
                if (c < 0 || world[r][c] == ' ')
                {
                    c = std::distance(world[r].cbegin(),
                                      std::find_if(world[r].crbegin(),
                                                   world[r].crend(),
                                                   [&](char const x) {
                                                       return x != ' ';
                                                   })
                                              .base()
                                          - 1);
                }
                break;

            case Dir::right:
                ++c;
                if (c >= world[r].size() || world[r][c] == ' ')
                {
                    c = std::distance(world[r].cbegin(),
                                      std::find_if(world[r].cbegin(),
                                                   world[r].cend(),
                                                   [&](char const x) {
                                                       return x != ' ';
                                                   }));
                }
                break;
        }

        return {r, c};
    };

    auto b = instr.cbegin();
    auto e = instr.cend();
    while (b != e)
    {
        auto const next = std::find_if(b, e, ::isalpha);
        int len = stoi(std::string{b, e});

        while (len--)
        {
            auto const nextCell = cellIndex(r, c, dir);
            if (world[nextCell.first][nextCell.second] == '#')
            {
                break;
            }
            else
            {
                r = nextCell.first;
                c = nextCell.second;
            }
        }

        b = next;
        if (b != e)
        {
            char const action = *b++;
            if (action == 'L')
            {
                dir = static_cast<Dir>((static_cast<int>(dir) + 3) % 4);
            }
            else
            {
                dir = static_cast<Dir>((static_cast<int>(dir) + 1) % 4);
            }
        }
    }

    std::cout << (1000 * (r + 1) + 4 * (c + 1) + static_cast<int>(dir)) << "\n";
}
