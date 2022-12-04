// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

int main()
{
    std::cout << std::count_if(
        std::istream_iterator<std::string>(std::cin),
        std::istream_iterator<std::string>(),
        [](std::string const& s) {
            int lfrom, lto, rfrom, rto;
            sscanf(s.c_str(), "%i-%i,%i-%i", &lfrom, &lto, &rfrom, &rto);
            return (lfrom >= rfrom && lto <= rto) || (rfrom >= lfrom && rto <= lto);
        }) << "\n";
}
