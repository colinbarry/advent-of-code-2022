// g++ -std=c++17 part1.cpp && ./a.out < test
#include <iostream>
#include <string>
#include <vector>

/** True if all elements in the given range are unique.
 */
template <typename It>
bool unique_elements(It b, It e)
{
    while (b != e)
    {
        if (std::find(std::next(b), e, *b) != e)
            return false;

        b = std::next(b);
    }

    return true;
}

int main()
{
    std::string s;
    std::getline(std::cin, s);

    std::vector<char> buff{s.cbegin(), s.cbegin() + 4};
    auto const it = std::find_if(s.cbegin() + 4, s.cend(), [&buff](char const c) {
        if (unique_elements(buff.cbegin(), buff.cend()))
        {
            return true;
        }
        else
        {
            buff[0] = c;
            std::rotate(buff.begin(), buff.begin() + 1, buff.end());
            return false;
        }
    });

    std::cout << std::distance(s.cbegin(), it) << "\n";
}
