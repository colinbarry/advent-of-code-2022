// g++ -std=c++17 part1.cpp && ./a.out < test
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

    auto const checkVisibility = [&](int const x, int const y) {
        const int treeHeight = forest[y][x];

        auto const checkRow = [&](int const row, int bx, int const ex) {
            for (; bx < ex; ++bx)
            {
                if (forest[row][bx] >= treeHeight)
                    return false;
            }

            return true;
        };

        auto const checkColumn = [&](int const column, int by, int const ey) {
            for (; by < ey; ++by)
            {
                if (forest[by][column] >= treeHeight)
                    return false;
            }

            return true;
        };

        return checkRow(y, 0, x) || checkRow(y, x + 1, width)
               || checkColumn(x, 0, y) || checkColumn(x, y + 1, height);
    };

    int visibleTrees = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (checkVisibility(x, y))
                ++visibleTrees;
        }
    }

    std::cout << "=" << visibleTrees << "\n";
}
