// g++ -std=c++17 part2.cpp && ./a.out < test
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

char computeCommon(std::string a, std::string b, std::string c)
{
    auto const sanitise = [](std::string s) {
        std::sort(s.begin(), s.end());
        s.erase(std::unique(s.begin(), s.end()), s.end());
        return s;
    };

    a = sanitise(a);
    b = sanitise(b);
    c = sanitise(c);

    std::vector<char> abCommon, abcCommon;
    std::set_intersection(a.cbegin(),
                          a.cend(),
                          b.cbegin(),
                          b.cend(),
                          std::back_inserter(abCommon));
    std::set_intersection(abCommon.cbegin(),
                          abCommon.cend(),
                          c.cbegin(),
                          c.cend(),
                          std::back_inserter(abcCommon));

    return abcCommon[0];
}

int main()
{
    int score = 0;

    std::string a, b, c;
    while (std::cin >> a >> b >> c)
    {
        char const common = computeCommon(a, b, c);
        score += (common >= 'a' && common <= 'z') ? (1 + common - 'a')
                                                  : (27 + common - 'A');
    }

    std::cout << score << "\n";
}
