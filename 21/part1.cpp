// g++ -std=c++17 part1.cpp && ./a.out < test
#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <string_view>

struct Monkey
{
    std::optional<int64_t> value;
    std::function<int64_t(int64_t, int64_t)> fn;
    std::string leftOperand, rightOperand;
};

std::map<std::string, Monkey> monkeys;

int64_t eval(std::string const& name)
{
    Monkey& monkey = monkeys.find(name)->second;
    if (!monkey.value)
        monkey.value = monkey.fn(eval(monkey.leftOperand),
                                 eval(monkey.rightOperand));

    return *monkey.value;
}

Monkey makeMonkey(std::string const& value)
{
    Monkey monkey;

    if (::isdigit(value[0]))
    {
        monkey.value = std::stoi(value);
    }
    else
    {
        static std::regex const re{"(\\w+) (.) (\\w+)"};
        std::smatch match;
        std::regex_search(value, match, re);

        monkey.leftOperand = match[1].str();
        char const op = match[2].str()[0];
        switch (op)
        {
            case '+':
                monkey.fn = std::plus<int64_t>{};
                break;
            case '-':
                monkey.fn = std::minus<int64_t>{};
                break;
            case '*':
                monkey.fn = std::multiplies<int64_t>{};
                break;
            case '/':
                monkey.fn = std::divides<int64_t>{};
                break;
        }
        monkey.rightOperand = match[3].str();
    }

    return monkey;
}

int main()
{
    std::regex const re{"([^:]+): (.+)"};
    std::string s;
    while (std::getline(std::cin, s))
    {
        std::smatch match;
        std::regex_match(s, match, re);

        std::string const label = match[1].str();
        std::string const value = match[2].str();

        monkeys.insert({label, makeMonkey(value)});
    }

    std::cout << eval("root") << "\n";
}
