// g++ -std=c++17 part2.cpp && ./a.out < test
#include <algorithm>
#include <array>
#include <iostream>
#include <string>

int main()
{
    int x = 1;
    int cycle = 1;
    std::array<std::array<char, 40>, 6> crt;

    auto const draw = [&] {
        int const row = (cycle - 1) / 40;
        int const col = (cycle - 1) % 40;
        bool const on = x >= col - 1 && x <= col + 1;
        crt[row][col] = on ? '#' : '.';
    };

    std::string s;
    while (std::getline(std::cin, s))
    {
        draw();
        ++cycle;
        if (s != "noop")
        {
            int v;
            sscanf(s.c_str(), "addx %i", &v);

            draw();
            ++cycle;
            x += v;
        }
    }

    for (auto const& row : crt)
    {
        for (char c : row)
            std::cout << c;
        std::cout << "\n";
    }
}
