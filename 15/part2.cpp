// g++ -std=c++17 part2.cpp && ./a.out < test
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

struct Sensor
{
    int x, y;
    struct
    {
        int x, y;
    } beacon;
};

std::pair<int, int> rowRange(Sensor const& sensor, int const row)
{
    int const distance = std::abs(sensor.x - sensor.beacon.x)
                         + std::abs(sensor.y - sensor.beacon.y);
    int const dy = std::abs(sensor.y - row);

    if (dy > distance)
    {
        return {0, 0};
    }
    else
    {
        return {sensor.x - distance + dy, sensor.x + distance - dy + 1};
    }
}

int main()
{
    std::vector<Sensor> sensors;
    std::string s;
    while (std::getline(std::cin, s))
    {
        Sensor sensor;
        sscanf(s.c_str(),
               "Sensor at x=%i, y=%i: closest beacon is at x=%i, y=%i",
               &sensor.x,
               &sensor.y,
               &sensor.beacon.x,
               &sensor.beacon.y);
        sensors.push_back(sensor);
    }

    static constexpr int searchRange = 4000000;
    for (int row = 0; row < searchRange + 1; ++row)
    {
        std::vector<std::pair<int, int>> ranges;
        std::vector<int> possibles;
        for (auto const& sensor : sensors)
        {
            auto range = rowRange(sensor, row);
            ranges.push_back(range);

            if (range.first > 0)
                possibles.push_back(range.first - 1);
            if (range.second < searchRange)
                possibles.push_back(range.second + 1);
        }

        for (int const x : possibles)
        {
            bool const ok = std::none_of(ranges.cbegin(),
                                         ranges.cend(),
                                         [&](auto const& each) {
                                             return x >= each.first
                                                    && x < each.second;
                                         });

            if (ok)
            {
                std::cout << "=" << (row + static_cast<int64_t>(x) * 4000000)
                          << "\n";
                exit(EXIT_SUCCESS);
            }
        }
    }
}
