// g++ -std=c++17 part2.cpp && ./a.out < test
#include <algorithm>
#include <deque>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// To keep memory usage down, replace each location, such as AA with a
// single `byte`.
using byte = uint8_t;

struct Room
{
    std::string label;
    int flowRate;
    std::vector<byte> destinations;
};

std::unordered_map<byte, Room> rooms;
using Distances = std::unordered_map<byte, int>;
std::unordered_map<byte, Distances> distances;

/** Returns the shortest distance from source to every room.
 */
std::unordered_map<byte, int> dijkstra(byte source)
{
    std::unordered_map<byte, int> dists;

    std::transform(rooms.cbegin(),
                   rooms.cend(),
                   std::inserter(dists, dists.end()),
                   [&](auto const& each) -> decltype(dists)::value_type {
                       return {each.first,
                               source == each.first
                                   ? 0
                                   : std::numeric_limits<int>::max()};
                   });

    std::unordered_set<byte> unvisited;
    std::transform(rooms.cbegin(),
                   rooms.cend(),
                   std::inserter(unvisited, unvisited.end()),
                   [](auto const& each) { return each.first; });

    while (!unvisited.empty())
    {
        // find unvisited node with lowest distance.
        byte const u = std::accumulate(std::next(unvisited.cbegin()),
                                       unvisited.cend(),
                                       *unvisited.begin(),
                                       [&](auto const& acc, auto const& each) {
                                           return dists[acc] < dists[each] ? acc
                                                                           : each;
                                       });

        auto const& room = rooms[u];
        int const distance = dists[u];

        // update the shortest distance to all unvisited nodes adjacent to u
        for (byte next : room.destinations)
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
std::unordered_map<byte, int> filter(std::unordered_map<byte, int> const& shortestDistances)
{
    std::unordered_map<byte, int> remains;

    std::copy_if(shortestDistances.cbegin(),
                 shortestDistances.cend(),
                 std::inserter(remains, remains.end()),
                 [](auto const& each) {
                     return each.second > 0 && rooms[each.first].flowRate > 0;
                 });

    return remains;
}

static constexpr int maxTime = 26;

/** Returns the best score by performing a breadth-first search. Possibilities
 * are pruned in the observation that the only good move would be to move to
 * a room with a working valve that is currently closed, and open it.
 */
int findBestScore(byte startRoom)
{
    struct Move
    {
        int time;
        int score;
        byte location;
        byte elephant;
        int meBusyUntil;
        int elephantBusyUntil;
        std::vector<byte> remainingFlows;
    };

    int bestScore = 0;
    std::deque<Move> moves;

    moves.push_back(
        {0, 0, startRoom, startRoom, 0, 0, ([&] {
             std::vector<byte> remainingFlows;
             auto const& locs = distances[startRoom];
             std::transform(locs.cbegin(),
                            locs.cend(),
                            std::back_inserter(remainingFlows),
                            [](auto const& each) { return each.first; });
             return remainingFlows;
         }())});

    while (!moves.empty())
    {
        Move move = moves.front();
        moves.pop_front();

        bestScore = std::max(move.score, bestScore);

        bool meToMakeAMove = move.meBusyUntil == move.time;
        bool elephantToMakeAMove = move.elephantBusyUntil == move.time;

        // Returns a candidate moves from the given location as a vector of
        // <label, distance> pairs.
        auto const candidates = [&](byte const label) {
            auto const room = rooms.find(label);
            using LocationAndDuration = std::pair<byte, int>;
            std::vector<LocationAndDuration> candidates;
            for (auto const& candidate : distances[label])
            {
                if (std::find(move.remainingFlows.cbegin(),
                              move.remainingFlows.cend(),
                              candidate.first)
                    != move.remainingFlows.cend())
                {
                    candidates.push_back({candidate.first, candidate.second + 1});
                }
            };
            return candidates;
        };

        if (meToMakeAMove)
        {
            for (auto const& candidate : candidates(move.location))
            {
                if (move.time + candidate.second >= maxTime)
                    continue;

                Move nextMove = move;
                nextMove.meBusyUntil = nextMove.time + candidate.second;
                nextMove.time = std::min(nextMove.elephantBusyUntil,
                                         nextMove.meBusyUntil);
                nextMove.location = candidate.first;

                nextMove.remainingFlows
                    .erase(std::remove(nextMove.remainingFlows.begin(),
                                       nextMove.remainingFlows.end(),
                                       nextMove.location),
                           nextMove.remainingFlows.end());

                auto const& nextRoom = rooms[candidate.first];
                nextMove.score += nextRoom.flowRate
                                  * (maxTime - nextMove.meBusyUntil);

                moves.push_back(nextMove);
            }

            if (move.elephantBusyUntil < maxTime)
            {
                Move nextMove = move;
                nextMove.meBusyUntil = maxTime;
                nextMove.time = std::min(nextMove.elephantBusyUntil,
                                         nextMove.meBusyUntil);
                moves.push_back(nextMove);
            }
        }

        else if (elephantToMakeAMove)
        {
            for (auto const& candidate : candidates(move.elephant))
            {
                if (move.time + candidate.second >= maxTime)
                    continue;

                Move nextMove = move;
                nextMove.elephantBusyUntil = nextMove.time + candidate.second;
                nextMove.time = std::min(nextMove.elephantBusyUntil,
                                         nextMove.meBusyUntil);
                nextMove.elephant = candidate.first;
                nextMove.remainingFlows
                    .erase(std::remove(nextMove.remainingFlows.begin(),
                                       nextMove.remainingFlows.end(),
                                       nextMove.elephant),
                           nextMove.remainingFlows.end());

                auto const& nextRoom = rooms[candidate.first];
                nextMove.score += nextRoom.flowRate
                                  * (maxTime - nextMove.elephantBusyUntil);

                moves.push_back(nextMove);
            }

            if (move.meBusyUntil < maxTime)
            {
                Move nextMove = move;
                nextMove.elephantBusyUntil = maxTime;
                nextMove.time = std::min(nextMove.elephantBusyUntil,
                                         nextMove.meBusyUntil);
                moves.push_back(nextMove);
            }
        }
    }

    return bestScore;
}

int main()
{
    std::regex const re{
        R"(Valve (\w\w) has flow rate=(\d+); tunnels? leads? to valves? (.*))"};
    std::smatch match;

    std::unordered_map<std::string, byte> roomToId;
    byte roomIndex = 0;

    auto const getRoomId = [&](std::string const& label) {
        auto const it = roomToId.find(label);
        if (it == roomToId.cend())
        {
            byte const index = roomIndex++;
            roomToId.insert(std::make_pair(label, index));
            return index;
        }
        else
        {
            return it->second;
        }
    };

    std::string s;
    while (std::getline(std::cin, s))
    {
        std::regex_match(s, match, re);
        std::string const label = match[1];
        byte const thisRoomIndex = getRoomId(label);

        Room room;
        room.label = label;
        room.flowRate = std::stoi(match[2]);

        std::istringstream dests(match[3]);
        while (std::getline(dests, s, ','))
        {
            if (s[0] == ' ')
                s = s.substr(1);

            auto const it = roomToId.find(s);
            byte const destRoomIndex = getRoomId(s);
            room.destinations.push_back(destRoomIndex);
        }

        rooms.insert(std::make_pair(thisRoomIndex, std::move(room)));
    }
    //
    // Use Dijkstra's shortest path algo to calculate shortest distance to
    // each (non-broken) room from the start, and from every other
    // non-broken room.
    byte const aaId = roomToId["AA"];
    distances[aaId] = filter(dijkstra(aaId));
    for (auto const& room : rooms)
    {
        if (room.second.flowRate > 0)
            distances[room.first] = filter(dijkstra(room.first));
    }

    std::cout << "#" << findBestScore(aaId) << "\n";
}
