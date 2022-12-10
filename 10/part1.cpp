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
        if (s == "noop")
        {
            ++cycle;
            if (isInteresting(cycle))
                strength += cycle * x;
        }
        else
        {
            int v;
            sscanf(s.c_str(), "addx %i", &v);

            ++cycle;
            if (isInteresting(cycle))
                strength += cycle * x;

            x += v;
            ++cycle;
            if (isInteresting(cycle))
                strength += cycle * x;
        }
    }

    std::cout << strength << "\n";
}
