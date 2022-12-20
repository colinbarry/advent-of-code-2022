// g++ -std=c++17 part2.cpp && ./a.out < test
#include <array>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>

using RockDef = std::vector<std::pair<int, int>>;

static const std::array<RockDef, 5> rockDefs
    = {RockDef{{0, 0}, {1, 0}, {2, 0}, {3, 0}},
       RockDef{{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}},
       RockDef{{2, 0}, {2, 1}, {0, 2}, {1, 2}, {2, 2}},
       RockDef{{0, 0}, {0, 1}, {0, 2}, {0, 3}},
       RockDef{{0, 0}, {0, 1}, {1, 0}, {1, 1}}};

struct Rock
{
    RockDef blocks;
    int height;
};

std::array<Rock, 5> rocks;

int main()
{
    std::transform(rockDefs.cbegin(), rockDefs.cend(), rocks.begin(), [](RockDef const& def) {
        return Rock{def,
                    std::accumulate(def.cbegin(),
                                    def.cend(),
                                    0,
                                    [](int const height, auto const& pair) {
                                        return std::max(height, pair.second + 1);
                                    })};
    });

    std::string input;
    std::getline(std::cin, input);

    std::set<std::pair<int, int>> fixed;

    auto const top = [&] {
        return std::accumulate(fixed.cbegin(),
                               fixed.cend(),
                               0,
                               [](int const height, auto const& pair) {
                                   return std::min(height, pair.second);
                               });
    };

    auto const testSpace = [&](int const x, int const y) {
        return x >= 0 && x < 7 && y < 0 && fixed.count(std::make_pair(x, y)) == 0;
    };

    bool spawnNewRock = true;
    int currRock = 0;
    int x, y;
    int numSettledRocks = 0;
    int moveIndex = 0;

    while (true)
    {
        const char move = input[moveIndex];
        moveIndex = (moveIndex + 1) % input.size();

        if (spawnNewRock)
        {
            x = 2;
            y = top() - 3 - rocks[currRock].height;
            spawnNewRock = false;
        }

        Rock const& rock = rocks[currRock];

        auto const canMove =
            [&](int const dx, int const dy) {
                return std::all_of(rock.blocks.cbegin(),
                                  rock.blocks.cend(),
                                  [&](auto const& block) {
                                      return testSpace(x + block.first + dx,
                                                       y + block.second + dy);
                                  });
            };

        if (move == '<')
        {
            if (canMove(-1, 0))
                x--;
        }
        else
        {
            if (canMove(1, 0))
                x++;
        }

        if (canMove(0, 1))
        {
            ++y;
        }
        else
        {
            for (auto const& block : rock.blocks)
                fixed.insert(std::make_pair(block.first + x, block.second + y));

            if (++numSettledRocks == 2022)
            {
                std::cout << -top();
                exit(EXIT_SUCCESS);
            }

            spawnNewRock = true;
            currRock = (currRock + 1) % rocks.size();
        }
    }
}
