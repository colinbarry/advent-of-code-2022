// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
#include <cstdio>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main()
{
    std::string s;
    std::map<size_t, std::deque<char>> columns;

    while (std::getline(std::cin, s))
    {
        if (std::find(s.cbegin(), s.cend(), '[') == s.cend())
            break;

        for (size_t i = 1, index = 1; i < s.size(); i += 4, ++index)
        {
            char const c = s[i];
            if (::isalpha(c))
            {
                columns[index].push_front(c);
            }
        }
    }

    std::string skipline;
    std::getline(std::cin, skipline);

    while (std::getline(std::cin, s))
    {
        int num, from, to;
        sscanf(s.c_str(), "move %i from %i to %i", &num, &from, &to);

        auto& fromColumn = columns[from];
        auto& toColumn = columns[to];

        std::vector<char> buffer;
        std::copy(fromColumn.cend() - num,
                  fromColumn.cend(),
                  std::back_inserter(buffer));
        std::reverse(buffer.begin(), buffer.end());
        fromColumn.erase(fromColumn.end() - num, fromColumn.end());

        toColumn.insert(toColumn.end(), buffer.cbegin(), buffer.cend());
    }

    for (auto&& each : columns)
    {
        std::cout << each.second.back();
    }
    std::cout << "\n";
}
