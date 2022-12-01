// g++ -std=c++17 part1.cpp && ./a.out < test
#include <string>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> totals;

    std::string s;
    int sum = 0;
    while (std::getline(std::cin, s)) {
        if (s.empty()) {
            totals.push_back(sum);
            sum = 0;
        } else {
            sum +=  std::stoi(s);
        }
    }

    if (sum)
        totals.push_back(sum);

    std::cout << *std::max_element(totals.cbegin(), totals.cend()) << "\n";
}

