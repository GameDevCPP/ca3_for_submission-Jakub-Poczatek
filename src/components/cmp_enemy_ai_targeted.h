#pragma once
#include "cmp_actor_movement.h"
#include <engine.h>

class EnemyTargetedAIComponent : public ActorMovementComponent {
protected:
    sf::Vector2f _direction;
    std::weak_ptr<Entity> _player;

public:
    void update(double dt) override;

    explicit EnemyTargetedAIComponent(Entity* p);
    EnemyTargetedAIComponent() = delete;
};
