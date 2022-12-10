// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <string>

int sign(int const x)
{
    if (x > 0)
    {
        return 1;
    }
    else if (x < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int main()
{
    std::string s;
    std::array<std::pair<int, int>, 10> rope = {};
    std::set<std::pair<int, int>> tails;
    tails.insert(rope.back());
    while (std::getline(std::cin, s))
    {
        char cmd;
        int num;
        sscanf(s.c_str(), "%c %i", &cmd, &num);

        auto const move = [&](int const dx, int const dy) {
            rope.front().first += dx;
            rope.front().second += dy;

            for (int i = 1; i < rope.size(); ++i)
            {
                auto& part = rope[i];
                auto& prevPart = rope[i - 1];
                if (std::abs(part.first - prevPart.first) > 1
                    || std::abs(part.second - prevPart.second) > 1)
                {
                    int const ox = sign(prevPart.first - part.first);
                    int const oy = sign(prevPart.second - part.second);
                    part.first += ox;
                    part.second += oy;
                }
            }

            tails.insert(rope.back());
        };

        for (int i = 0; i < num; ++i)
        {
            switch (cmd)
            {
                case 'U':
                    move(0, -1);
                    break;
                case 'D':
                    move(0, 1);
                    break;
                case 'L':
                    move(-1, 0);
                    break;
                case 'R':
                    move(1, 0);
                    break;
            }
        }
    }

    std::cout << tails.size() << "\n";
}
