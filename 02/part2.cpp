// g++ -std=c++17 part2.cpp && ./a.out < test
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

const std::unordered_map<char, Move> moveMapping = {{'A', Move::rock},
                                                    {'B', Move::paper},
                                                    {'C', Move::scissors}};

const std::unordered_map<char, Result> resultMapping = {{'X', Result::lose},
                                                        {'Y', Result::draw},
                                                        {'Z', Result::win}};

Move myMove(Result const result, Move const otherMove)
{
    if (result == Result::draw)
    {
        return otherMove;
    }
    else if (result == Result::win)
    {
        switch (otherMove)
        {
            case Move::rock:
                return Move::paper;
            case Move::paper:
                return Move::scissors;
            case Move::scissors:
                return Move::rock;
        }
    }
    else
    {
        switch (otherMove)
        {
            case Move::rock:
                return Move::scissors;
            case Move::paper:
                return Move::rock;
            case Move::scissors:
                return Move::paper;
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
    std::vector<std::pair<Move, Result>> turns;

    while (std::getline(std::cin, s))
    {
        std::istringstream ss(s);
        char other, me;
        ss >> other >> me;
        turns.push_back(
            {moveMapping.find(other)->second, resultMapping.find(me)->second});
    }

    std::cout << std::accumulate(
        turns.cbegin(), turns.cend(), 0, [](int const score, auto const turn) {
            return score + static_cast<int>(myMove(turn.second, turn.first)) + 1
                   + resultPoints(turn.second);
        }) << "\n";
}
