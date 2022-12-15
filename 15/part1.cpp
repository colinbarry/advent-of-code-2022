// g++ -std=c++17 part2.cpp && ./a.out < test
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <set>

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

    std::set<int> used;
    static constexpr int row = 2000000;
    for (auto const& sensor : sensors)
    {
        auto range = rowRange(sensor, row);
        for (int i = range.first; i < range.second; ++i) {
            used.insert(i);
        }
    }

    for (auto const& sensor : sensors)
    {
        if (sensor.beacon.y == row) {
            used.erase(sensor.beacon.x);
        }
    }

    std::cout << used.size() << "\n";

}
