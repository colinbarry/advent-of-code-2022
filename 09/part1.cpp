// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
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
    int hx = 0, hy = 0;
    int tx = 0, ty = 0;
    std::set<std::pair<int, int>> tails;
    tails.insert(std::pair{tx, ty});
    while (std::getline(std::cin, s))
    {
        char cmd;
        int num;
        sscanf(s.c_str(), "%c %i", &cmd, &num);

        auto const move = [&](int const dx, int const dy) {
            hx += dx;
            hy += dy;

            if (std::abs(tx - hx) > 1 || std::abs(ty - hy) > 1)
            {
                int const ox = sign(hx - tx);
                int const oy = sign(hy - ty);
                tx += ox;
                ty += oy;
            }

            tails.insert(std::pair{tx, ty});
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
