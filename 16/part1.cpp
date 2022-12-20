// g++ -std=c++17 part2.cpp && ./a.out < test
#include <algorithm>
#include <deque>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Room
{
    int flowRate;
    std::vector<std::string> destinations;
};

std::unordered_map<std::string, Room> rooms;
using Distances = std::unordered_map<std::string, int>;
std::unordered_map<std::string, Distances> distances;

/** Returns the shortest distance from source to every room.
 */
std::unordered_map<std::string, int> dijkstra(std::string const& source)
{
    std::unordered_map<std::string, int> dists;

    std::transform(rooms.cbegin(),
                   rooms.cend(),
                   std::inserter(dists, dists.end()),
                   [&](auto const& each) -> decltype(dists)::value_type {
                       return {each.first,
                               source == each.first
                                   ? 0
                                   : std::numeric_limits<int>::max()};
                   });

    std::unordered_set<std::string> unvisited;
    std::transform(rooms.cbegin(),
                   rooms.cend(),
                   std::inserter(unvisited, unvisited.end()),
                   [](auto const& each) { return each.first; });

    while (!unvisited.empty())
    {
        // find unvisited node with lowest distance.
        std::string const u = std::accumulate(std::next(unvisited.cbegin()),
                                              unvisited.cend(),
                                              *unvisited.begin(),
                                              [&](auto const& acc, auto const& each) {
                                                  return dists[acc] < dists[each]
                                                             ? acc
                                                             : each;
                                              });

        auto const& room = rooms[u];
        int const distance = dists[u];

        // update the shortest distance to all unvisited nodes adjacent to u
        for (std::string const& next : room.destinations)
        {
            if (unvisited.count(next))
                dists[next] = std::min(dists[next], distance + 1);
        }

        unvisited.erase(u);
    }

    return dists;
}

/** Filters the distances to include only rooms with working valves and to
 * exclude 0 length routes (i.e., source to source).
 */
std::unordered_map<std::string, int> filter(
    std::unordered_map<std::string, int> const& shortestDistances)
{
    std::unordered_map<std::string, int> remains;

    std::copy_if(shortestDistances.cbegin(),
                 shortestDistances.cend(),
                 std::inserter(remains, remains.end()),
                 [](auto const& each) {
                     return each.second > 0 && rooms[each.first].flowRate > 0;
                 });

    return remains;
}

static constexpr int maxTurns = 30;

/** Returns the best score by performing a breadth-first search. Possibilities
 * are pruned in the observation that the only good move would be to move to
 * a room with a working valve that is currently closed, and open it.
 */
int findBestScore()
{
    struct Move
    {
        int turn;
        int score;
        std::string location;
        std::vector<std::string> onFlows;
    };

    int bestScore = 0;
    std::deque<Move> moves;
    moves.push_back({0, 0, "AA"});

    while (!moves.empty())
    {
        Move move = moves.front();
        moves.pop_front();

        bestScore = std::max(bestScore, move.score);

        auto const location = rooms.find(move.location);
        assert(location != rooms.cend());

        auto const& distancesFromHere = distances[move.location];

        for (auto const& candidate : distancesFromHere)
        {
            if (1 + move.turn + candidate.second < maxTurns
                && std::find(move.onFlows.cbegin(),
                             move.onFlows.cend(),
                             candidate.first)
                       == move.onFlows.cend())
            {
                Move nextMove = move;
                nextMove.turn += candidate.second + 1;
                nextMove.location = candidate.first;
                nextMove.onFlows.push_back(nextMove.location);

                auto const& nextRoom = rooms[nextMove.location];
                nextMove.score += nextRoom.flowRate * (maxTurns - nextMove.turn);

                moves.push_back(nextMove);
            }
        };
    }

    return bestScore;
}

int main()
{
    std::regex const re{
        R"(Valve (\w\w) has flow rate=(\d+); tunnels? leads? to valves? (.*))"};
    std::smatch match;

    std::string s;
    while (std::getline(std::cin, s))
    {
        std::regex_match(s, match, re);
        std::string const label = match[1];

        Room room;
        room.flowRate = std::stoi(match[2]);

        std::istringstream dests(match[3]);
        while (std::getline(dests, s, ','))
        {
            if (s[0] == ' ')
                s = s.substr(1);
            room.destinations.push_back(s);
        }

        rooms.insert(std::make_pair(label, std::move(room)));
    }

    // Use Dijkstra's shortest path algo to calculate shortest distance to
    // each (non-broken) room from the start, and from every other
    // non-broken room.
    distances["AA"] = filter(dijkstra("AA"));
    for (auto const& room : rooms)
    {
        if (room.second.flowRate > 0)
            distances[room.first] = filter(dijkstra(room.first));
    }

    std::cout << "#" << findBestScore() << "\n";
}
