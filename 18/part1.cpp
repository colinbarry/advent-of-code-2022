// g++ -std=c++17 part1.cpp && ./a.out < test
#include <array>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <tuple>

using Point = std::tuple<int, int, int>;

int main()
{
    std::set<Point> points;

    std::string s;
    while (std::getline(std::cin, s))
    {
        int x, y, z;
        sscanf(s.c_str(), "%i,%i,%i", &x, &y, &z);
        points.insert({x, y, z});
    }

    auto const exists = [&](int const x, int const y, int const z) {
        return points.count({x, y, z}) == 1;
    };

    static constexpr std::array<Point, 6> offsets = {Point{-1, 0, 0},
                                                     Point{1, 0, 0},
                                                     Point{0, -1, 0},
                                                     Point{0, 1, 0},
                                                     Point{0, 0, -1},
                                                     Point{0, 0, 1}};

    std::cout << std::accumulate(
        points.cbegin(), points.cend(), 0, [&](int const acc, auto const& each) {
            return acc
                   + std::count_if(offsets.cbegin(), offsets.cend(), [&](Point const& offset) {
                         return !exists(std::get<0>(offset) + std::get<0>(each),
                                        std::get<1>(offset) + std::get<1>(each),
                                        std::get<2>(offset) + std::get<2>(each));
                     });
        }) << "\n";
}
