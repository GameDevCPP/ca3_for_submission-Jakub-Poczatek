#pragma once

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstdlib>

struct GridLocation {
    int x, y;
};

// Helpers for GridLocation
inline bool operator == (GridLocation a, GridLocation b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator != (GridLocation a, GridLocation b) {
    return !(a == b);
}

inline bool operator < (GridLocation a, GridLocation b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

inline std::basic_iostream<char>::basic_ostream& operator<<(std::basic_iostream<char>::basic_ostream& out, const GridLocation& loc) {
    out << '(' << loc.x << ',' << loc.y << ')';
    return out;
}

namespace std {
/* implement hash function so we can put GridLocation into an unordered_set */
    template <> struct hash<GridLocation> {
        std::size_t operator()(const GridLocation& id) const noexcept {
            // NOTE: better to use something like boost hash_combine
            return std::hash<int>()(id.x ^ (id.y << 16));
        }
    };
}

class AStar{
private:
    template<typename T, typename priority_t>
    struct PriorityQueue{
        typedef std::pair<priority_t, T> PQElement;
        std::priority_queue<PQElement, std::vector<PQElement>,
            std::greater<PQElement>> elements;

        [[nodiscard]] inline bool empty() const{ return elements.empty(); }
        inline void put(T item, priority_t priority) {
            elements.emplace(priority, item);
        }
        inline T get() {
            T best_item = elements.top().second;
            elements.pop();
            return best_item;
        }
    };

    static double heuristic(GridLocation a, GridLocation b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

public:
    struct SquareGrid{
        static std::array<GridLocation, 8> DIRS;
        int _width, _height;
        std::unordered_set<GridLocation> _walls;

        SquareGrid(int width, int height);
        bool inBounds(GridLocation id) const;
        bool passable(GridLocation id) const;
        std::vector<GridLocation> neighbors(GridLocation id) const;
    };

    struct GridWithWeights: SquareGrid{
        std::unordered_set<GridLocation> _forests;

        GridWithWeights(int w, int h);
        double cost(GridLocation fromNode, GridLocation toNode) const;
    };

    template<typename Location>
    static std::vector<Location> reconstructPath(Location start,
          Location goal, std::unordered_map<Location, Location> cameFrom){
        std::vector<Location> path;
        Location current = goal;
        if (cameFrom.find(goal) == cameFrom.end()){
            return path; // no path can be found
        }
        while(current != start) {
            path.push_back(current);
            current = cameFrom[current];
        }
        path.push_back(start); // optional
        std::reverse(path.begin(), path.end());
        return path;
    }

    template<typename Location, typename Graph>
    static void aStarSearch(Graph graph, Location start,
         Location goal, std::unordered_map<Location, Location>& cameFrom,
         std::unordered_map<Location, double>& costSoFar) {

        PriorityQueue<Location, double> frontier;
        frontier.put(start, 0);

        cameFrom[start] = start;
        costSoFar[start] = 0;

        while (!frontier.empty()) {
            Location current = frontier.get();

            if (current == goal) {
                break;
            }

            for (Location next : graph.neighbors(current)) {
                double new_cost = costSoFar[current] + graph.cost(current, next);
                if (costSoFar.find(next) == costSoFar.end()
                    || new_cost < costSoFar[next]) {
                    costSoFar[next] = new_cost;
                    double priority = new_cost + heuristic(next, goal);
                    frontier.put(next, priority);
                    cameFrom[next] = current;
                }
            }
        }
    }
};
