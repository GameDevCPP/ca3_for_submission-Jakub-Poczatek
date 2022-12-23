#include "cmp_enemy_aStar.h"

#include <cmath>

AStar::GridWithWeights EnemyAStarComponent::makeGrid(){
    AStar::GridWithWeights grid(ls::getWidth(), ls::getHeight());
    populateWalls(grid);
    return grid;
}

void EnemyAStarComponent::populateWalls(AStar::SquareGrid& grid){
    auto walls = ls::findTiles(sf::Color(ls::WALL));
    for (auto w: walls) {
        grid._walls.insert(GridLocation{
            static_cast<int>(w.x), static_cast<int>(w.y)});
    }
}

void EnemyAStarComponent::update(double dt) {
    _dtCounter+= dt;
    auto currentPoint = sf::Vector2i(_path[_pathCounter].x, _path[_pathCounter].y);
    auto currentPosition = sf::Vector2i(std::floor(_parent->getPosition().x/ls::getTileSize()),
                                        std::floor(_parent->getPosition().y/ls::getTileSize()));

    _direction.x = (currentPoint.x > currentPosition.x) ? 1 : -1;
    _direction.y = (currentPoint.y > currentPosition.y) ? 1 : -1;
    move(_direction * (float)(dt * _speed));
    ActorMovementComponent::update(dt);

    if(std::abs((float) currentPoint.x - _parent->getPosition().x/ls::getTileSize()) < 0.5f
    && std::abs((float) currentPoint.y - _parent->getPosition().y/ls::getTileSize()) < 0.5f
    && _pathCounter < _path.size()-1) {
    _pathCounter++;
    }

    if(_dtCounter > 0.5){
        calculatePath();
        _dtCounter = 0;
    }
}

void EnemyAStarComponent::calculatePath() {
    sf::Vector2f target;
    _grid = makeGrid();
    _path.clear();
    _pathCounter = 0;

    if (auto pl = _player.lock()) {
        target = sf::Vector2f(std::floor(pl->getPosition().x/ls::getTileSize()),
                              std::floor(pl->getPosition().y/ls::getTileSize()));
    }

    auto pos = sf::Vector2f(std::floor(_parent->getPosition().x/ls::getTileSize()),
                            std::floor(_parent->getPosition().y/ls::getTileSize()));

    GridLocation start = {static_cast<int>(pos.x), static_cast<int>(pos.y)};
    GridLocation goal = {static_cast<int>(target.x), static_cast<int>(target.y)};
    std::unordered_map<GridLocation, GridLocation> came_from;
    std::unordered_map<GridLocation, double> cost_so_far;
    AStar::aStarSearch(_grid, start, goal, came_from, cost_so_far);
    auto tempPath = AStar::reconstructPath(start, goal, came_from);
    if(!tempPath.empty()) std::copy(tempPath.begin(), tempPath.end(), std::back_inserter(_path));
}

EnemyAStarComponent::EnemyAStarComponent(Entity *p) :
        ActorMovementComponent(p), _grid(makeGrid()), _speed(100.f),
        _direction(1, 1), _pathCounter(0),
        _player(_parent->scene->ents.find("player")[0]), _dtCounter(0){
    calculatePath();
}