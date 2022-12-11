// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

struct Monkey
{
    std::vector<int64_t> items;
    std::function<int64_t(int64_t)> op;
    int test;
    int trueMonkey;
    int falseMonkey;
    int numInspected = 0;
};

std::vector<int64_t> parseItems(std::string_view s)
{
    std::vector<int64_t> items;

    auto b = std::next(std::find(s.cbegin(), s.cend(), ':'));
    while (b != s.cend())
    {
        auto const e = std::find(b, s.cend(), ',');
        std::string number{b, e};
        items.push_back(std::stoi(number));

        b = e != s.cend() ? std::next(e) : e;
    }

    return items;
}

std::function<int64_t(int64_t)> parseOperation(std::string_view s)
{
    auto const symbolit = std::find_if(s.cbegin(), s.cend(), [](char c) {
        return c == '+' || c == '*';
    });
    std::string const operand{symbolit + 2, s.cend()};

    if (operand == "old")
    {
        if (*symbolit == '+')
            return std::bind(std::plus<int64_t>{},
                             std::placeholders::_1,
                             std::placeholders::_1);
        else
            return std::bind(std::multiplies<int64_t>{},
                             std::placeholders::_1,
                             std::placeholders::_1);
    }
    else
    {
        int const num = stoi(operand);
        if (*symbolit == '+')
            return std::bind(std::plus<int64_t>{}, std::placeholders::_1, num);
        else
            return std::bind(std::multiplies<int64_t>{}, std::placeholders::_1, num);
    }
}

int parseTrailingNumber(std::string_view s)
{
    auto const b = std::find_if(s.cbegin(), s.cend(), ::isdigit);
    return std::stoi(std::string{b, s.cend()});
}

int main()
{
    std::string s;
    std::vector<Monkey> monkeys;
    while (std::getline(std::cin, s))
    {
        Monkey monkey;

        // Starting items: ...
        std::getline(std::cin, s);
        monkey.items = parseItems(s);

        // Operation: new = ...
        std::getline(std::cin, s);
        monkey.op = parseOperation(s);

        // Test: divisible by n
        std::getline(std::cin, s);
        monkey.test = parseTrailingNumber(s);

        // If true
        std::getline(std::cin, s);
        monkey.trueMonkey = parseTrailingNumber(s);

        // If false
        std::getline(std::cin, s);
        monkey.falseMonkey = parseTrailingNumber(s);

        // skip blank line
        std::getline(std::cin, s);

        monkeys.push_back(std::move(monkey));
    }

    for (int round = 0; round < 20; ++round)
    {
        for (Monkey& each : monkeys)
        {
            for (int64_t& item : each.items)
            {
                item = each.op(item) / 3;
                if (item % each.test == 0)
                {
                    monkeys[each.trueMonkey].items.push_back(item);
                }
                else
                {
                    monkeys[each.falseMonkey].items.push_back(item);
                }
            }

            each.numInspected += each.items.size();

            each.items.clear();
        }
    }

    std::nth_element(monkeys.begin(),
                     monkeys.begin() + 2,
                     monkeys.end(),
                     [](Monkey const& lhs, Monkey const& rhs) {
                         return lhs.numInspected > rhs.numInspected;
                     });

    std::cout << (monkeys[0].numInspected * monkeys[1].numInspected) << "\n";
}
