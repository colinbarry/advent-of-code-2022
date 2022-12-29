// g++ -std=c++17 part2.cpp && ./a.out < test
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using ValAndInitPos = std::pair<int64_t, int>;

static constexpr int factor = 811589153;

int main()
{
    std::string s;
    std::vector<ValAndInitPos> vec;

    int i = 0;
    while (std::getline(std::cin, s))
        vec.push_back({static_cast<int64_t>(stoi(s)) * factor, i++});

    for (int j = 0; j < 10; ++j)
    {
        for (int i = 0; i < vec.size(); ++i)
        {
            auto const it = std::find_if(vec.begin(), vec.end(), [&](auto const& each) {
                return each.second == i;
            });

            int const index = std::distance(vec.begin(), it);
            int64_t const val = it->first;
            int newPos = static_cast<int>((index + val)
                                          % static_cast<int>(vec.size() - 1));
            if (newPos == 0)
            {
                newPos = vec.size() - 1;
            }
            else if (newPos < 0)
            {
                newPos += vec.size() - 1;
            }

            if (newPos > index)
            {
                std::rotate(it, std::next(it), vec.begin() + newPos + 1);
            }
            else
            {
                std::rotate(vec.begin() + newPos, it, std::next(it));
            }
        }
    }

    int const zeroIndex
        = std::distance(vec.cbegin(),
                        std::find_if(vec.cbegin(), vec.cend(), [](auto&& each) {
                            return each.first == 0;
                        }));

    int64_t const result = vec[(zeroIndex + 1000) % vec.size()].first
                           + vec[(zeroIndex + 2000) % vec.size()].first
                           + vec[(zeroIndex + 3000) % vec.size()].first;

    std::cout << result << "\n";
}
