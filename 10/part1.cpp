// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
#include <iostream>
#include <string>

bool isInteresting(int const cycle)
{
    return (cycle - 20) % 40 == 0;
};

int main()
{
    int x = 1;
    int cycle = 1;
    int strength = 0;

    std::string s;
    while (std::getline(std::cin, s))
    {
        ++cycle;
        if (isInteresting(cycle))
            strength += cycle * x;

        if (s != "noop")
        {
            int v;
            sscanf(s.c_str(), "addx %i", &v);

            ++cycle;
            x += v;
            if (isInteresting(cycle))
                strength += cycle * x;
        }
    }

    std::cout << strength << "\n";
}
