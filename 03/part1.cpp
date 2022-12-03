// g++ -std=c++17 part1.cpp && ./a.out < test
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

static int score(std::string lhs, std::string rhs)
{
    auto const sanitise = [](std::string s) {
        std::sort(s.begin(), s.end());
        s.erase(std::unique(s.begin(), s.end()), s.end());
        return s;
    };

    lhs = sanitise(lhs);
    rhs = sanitise(rhs);

    std::vector<char> intersection;
    std::set_intersection(lhs.cbegin(),
                          lhs.cend(),
                          rhs.cbegin(),
                          rhs.cend(),
                          std::back_inserter(intersection));

    return std::accumulate(intersection.cbegin(),
                           intersection.cend(),
                           0,
                           [](int score, char const c) {
                               return score
                                      + (c >= 'a' && c <= 'z' ? (1 + c - 'a')
                                                              : (27 + c - 'A'));
                           });
}

int main()
{
    std::cout << std::accumulate(std::istream_iterator<std::string>(std::cin),
                                 std::istream_iterator<std::string>(),
                                 0,
                                 [](int const total, std::string const& line) {
                                     auto const midpoint = line.cbegin()
                                                           + line.size() / 2;
                                     return total
                                            + score({line.cbegin(), midpoint},
                                                    {midpoint, line.cend()});
                                 })
              << "\n";
}
