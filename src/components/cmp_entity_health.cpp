#include "cmp_entity_health.h"

EntityHealth::EntityHealth(Entity* p, int health)
    : Component(p), _health(health), _dtCounter(0) {}

void EntityHealth::adjustHealth(int amount) {
    if(!_cooldown){
        _cooldown = true;
        _health += amount;
        if(_health <= 0) _parent->setForDelete();
    }
}

void EntityHealth::update(double dt) {
    if(_cooldown){
        _dtCounter += dt;
        if(_dtCounter > 1.f){
            _cooldown = false;
            _dtCounter = 0;
        }
    }
}