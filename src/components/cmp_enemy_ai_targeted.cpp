#include "cmp_enemy_ai_targeted.h"

using namespace std;
using namespace sf;

void EnemyTargetedAIComponent::update(double dt) {
    if(auto pl = _player.lock()){
        if(_parent->getPosition().x > pl->getPosition().x) _direction.x = -1;
        else _direction.x = 1;
    }
    auto m = _direction * (float)(dt * _speed);
    if(validMove(_parent->getPosition() + m)){
       move(_direction * (float)(dt * _speed));
    }
    ActorMovementComponent::update(dt);
}

EnemyTargetedAIComponent::EnemyTargetedAIComponent(Entity* p)
    : ActorMovementComponent(p), _player(_parent->scene->ents.find("player")[0]) {
    _direction = Vector2f(1.0f, 0);
    _speed = 100.0f;
}
