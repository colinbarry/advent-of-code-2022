// g++ -std=c++17 part2.cpp && ./a.out < test
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<std::vector<int8_t>> forest;

    std::string s;
    while (std::getline(std::cin, s))
    {
        std::vector<int8_t> row;
        std::transform(s.cbegin(),
                       s.cend(),
                       std::back_inserter(row),
                       [](char const c) { return c - '0'; });
        forest.push_back(std::move(row));
    }

    int const width = forest.front().size();
    int const height = forest.size();

    auto const scoreTree = [&](int const x, int const y) {
        const int treeHeight = forest[y][x];

        auto const scoreLeft = [&](int x, int const y) {
            int count = 0;
            for (x = x - 1; x >= 0; --x)
            {
                ++count;
                if (forest[y][x] >= treeHeight)
                    break;
            }

            return count;
        };

        auto const scoreRight = [&](int x, int const y) {
            int count = 0;
            for (x = x + 1; x < width; ++x)
            {
                ++count;
                if (forest[y][x] >= treeHeight)
                    break;
            }

            return count;
        };

        auto const scoreUp = [&](int const x, int y) {
            int count = 0;
            for (y = y - 1; y >= 0; --y)
            {
                ++count;
                if (forest[y][x] >= treeHeight)
                    break;
            }

            return count;
        };

        auto const scoreDown = [&](int const x, int y) {
            int count = 0;
            for (y = y + 1; y < height; ++y)
            {
                ++count;
                if (forest[y][x] >= treeHeight)
                    break;
            }

            return count;
        };

        return scoreUp(x, y) * scoreRight(x, y) * scoreDown(x, y) * scoreLeft(x, y);
    };

    int bestScore = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
            bestScore = std::max(bestScore, scoreTree(x, y));
    }

    std::cout << "=" << bestScore << "\n";
}
