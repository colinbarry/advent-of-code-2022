// g++ -std=c++17 part2.cpp && ./a.out < test
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

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

    std::nth_element(totals.begin(),
                     totals.begin() + 3,
                     totals.end(),
                     std::greater<>{});


    std::cout << std::accumulate(totals.cbegin(), totals.cbegin() + 3, 0) << "\n";
}

