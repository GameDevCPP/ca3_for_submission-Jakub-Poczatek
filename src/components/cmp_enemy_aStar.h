#pragma once
#include "cmp_actor_movement.h"
#include "LevelSystem.h"
#include <engine.h>
#include "AStar.h"

class EnemyAStarComponent: public ActorMovementComponent {
protected:
    float _speed;
    int _pathCounter;
    double _dtCounter;
    AStar::GridWithWeights _grid;
    std::vector<GridLocation> _path;
    sf::Vector2f _direction;
    std::weak_ptr<Entity> _player;

    static AStar::GridWithWeights makeGrid();
    static void populateWalls(AStar::SquareGrid& grid);

public:
    void update(double dt) override;
    void calculatePath();

    explicit EnemyAStarComponent(Entity *p);
    EnemyAStarComponent() = delete;
};
