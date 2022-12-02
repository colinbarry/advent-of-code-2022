// g++ -std=c++17 part1.cpp && ./a.out < test
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum class Move
{
    rock,
    paper,
    scissors
};

enum class Result
{
    win,
    lose,
    draw
};

const std::unordered_map<char, Move> mapping = {{'A', Move::rock},
                                                {'B', Move::paper},
                                                {'C', Move::scissors},
                                                {'X', Move::rock},
                                                {'Y', Move::paper},
                                                {'Z', Move::scissors}};

Result result(Move const other, Move const me)
{
    if (other == me)
    {
        return Result::draw;
    }
    else
    {
        switch (me)
        {
            case Move::rock:
                return other == Move::scissors ? Result::win : Result::lose;
            case Move::paper:
                return other == Move::rock ? Result::win : Result::lose;
            case Move::scissors:
                return other == Move::paper ? Result::win : Result::lose;
        }
    }
}

int resultPoints(Result const result)
{
    switch (result)
    {
        case Result::lose:
            return 0;
        case Result::draw:
            return 3;
        case Result::win:
            return 6;
    }
}

int main()
{
    std::string s;
    std::vector<std::pair<Move, Move>> moves;

    while (std::getline(std::cin, s))
    {
        std::istringstream ss(s);
        char other, me;
        ss >> other >> me;
        moves.push_back({mapping.find(other)->second, mapping.find(me)->second});
    }

    std::cout << std::accumulate(
        moves.cbegin(), moves.cend(), 0, [](int const score, auto const move) {
            return score + static_cast<int>(move.second) + 1
                   + resultPoints(result(move.first, move.second));
        }) << "\n";
}
