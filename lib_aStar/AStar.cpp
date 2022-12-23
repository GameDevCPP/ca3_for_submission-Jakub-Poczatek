#include "AStar.h"

std::array<GridLocation, 8> AStar::SquareGrid::DIRS = {
        GridLocation{1, 0}, GridLocation{-1, 0},
        GridLocation{0, -1}, GridLocation{0, 1},
        GridLocation{1, 1}, GridLocation{-1, -1},
        GridLocation{1, -1}, GridLocation{-1, 1}

};

AStar::SquareGrid::SquareGrid(int width, int height)
    : _width(width), _height(height) {}

bool AStar::SquareGrid::inBounds(GridLocation id) const {
    return 0 <= id.x && id.x < _width
    && 0 <= id.y && id.y < _height;
}

bool AStar::SquareGrid::passable(GridLocation id) const {
    return _walls.find(id) == _walls.end();
}

std::vector<GridLocation> AStar::SquareGrid::neighbors(GridLocation id) const {
    std::vector<GridLocation> results;

    for (GridLocation dir: DIRS) {
        GridLocation next{id.x + dir.x, id.y + dir.y};
        if(inBounds(next) && passable(next))
            results.push_back(next);
    }

    if((id.x + id.y) % 2 == 0) {
        std::reverse(results.begin(), results.end());
    }

    return results;
}

AStar::GridWithWeights::GridWithWeights(int w, int h)
    : SquareGrid(w, h) {}

double AStar::GridWithWeights::cost(GridLocation fromNode, GridLocation toNode) const {
    return _forests.find(toNode) != _forests.end() ? 5 : 1;
}

