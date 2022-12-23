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

    static constexpr std::array<Point, 6> offsets = {Point{-1, 0, 0},
                                                     Point{1, 0, 0},
                                                     Point{0, -1, 0},
                                                     Point{0, 1, 0},
                                                     Point{0, 0, -1},
                                                     Point{0, 0, 1}};

    // Calculate enclosing hull
    struct Dimensions
    {
        int minx, maxx;
        int miny, maxy;
        int minz, maxz;
    };

    Dimensions const hull
        = std::accumulate(std::next(points.cbegin()),
                          points.cend(),
                          Dimensions{std::get<0>(*points.begin()),
                                     std::get<0>(*points.begin()),
                                     std::get<1>(*points.begin()),
                                     std::get<1>(*points.begin()),
                                     std::get<2>(*points.begin()),
                                     std::get<2>(*points.begin())},
                          [](Dimensions d, const Point& pt) {
                              d.minx = std::min(d.minx, std::get<0>(pt));
                              d.maxx = std::max(d.maxx, std::get<0>(pt));
                              d.miny = std::min(d.miny, std::get<1>(pt));
                              d.maxy = std::max(d.maxy, std::get<1>(pt));
                              d.minz = std::min(d.minz, std::get<2>(pt));
                              d.maxz = std::max(d.maxz, std::get<2>(pt));

                              return d;
                          });

    // Flood fill
    std::set<Point> const outer = ([&] {
        std::set<Point> outer;

        std::function<void(Point const&)> fill;
        fill = [&](Point const& pt) {
            int const x = std::get<0>(pt);
            int const y = std::get<1>(pt);
            int const z = std::get<2>(pt);
            if (x < hull.minx - 1 || x > hull.maxx + 1 || y < hull.miny - 1
                || y > hull.maxy + 1 || z < hull.minz - 1 || z > hull.maxz + 1
                || outer.count(pt) || points.count(pt))
                return;

            outer.insert(pt);

            for (auto const& offset : offsets)
            {
                fill({std::get<0>(offset) + x,
                      std::get<1>(offset) + y,
                      std::get<2>(offset) + z});
            }
        };

        fill({hull.minx - 1, hull.miny - 1, hull.minz - 1});

        return outer;
    }());

    auto const adjacentOuter = [&](int const x, int const y, int const z) {
        return outer.count({x, y, z}) == 1;
    };

    std::cout << std::accumulate(
        points.cbegin(), points.cend(), 0, [&](int const acc, auto const& each) {
            return acc + std::count_if(offsets.cbegin(), offsets.cend(), [&](Point const& offset) {
                       return adjacentOuter(std::get<0>(offset) + std::get<0>(each),
                                            std::get<1>(offset) + std::get<1>(each),
                                            std::get<2>(offset) + std::get<2>(each));
                   });
        }) << "\n";
}
